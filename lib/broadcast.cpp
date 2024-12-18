#include "broadcast.h"
#include "log.h"

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


namespace {

constexpr const char *BROADCAST_IP = "255.255.255.255";

int create_receiving_socket(uint16_t port) {
    sockaddr_in receiving_address;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    receiving_address = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = { .s_addr = htonl(INADDR_ANY) }
    };

    // Bind the socket to the address
    if (bind(sock, (struct sockaddr*) &receiving_address, sizeof(receiving_address)) < 0) {
        perror("Bind failed");
        close(sock);

        return -1;
    }

    int flags = fcntl(sock, F_GETFL, 0);

    if (flags == -1) {
        perror("fcntl");
        close(sock);

        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, flags) == -1) {
        perror("fcntl");
        close(sock);

        return -1;
    }

    return sock;
}

int create_broadcast_socket() {
    int broadcast_permission = 1;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");

        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_permission, sizeof(broadcast_permission)) < 0) {
        perror("Error enabling broadcast");
        close(sock);

        return -1;
    }

    return sock;
}


int create_peer2peer_socket() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");

        return -1;
    }

    return sock;
}

bool is_mine_address(const sockaddr_in& addr) {
    ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return false; // Handle error appropriately
    }

    for (ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != addr.sin_family) {
            continue;
        }

        if (ifa->ifa_addr->sa_family == AF_INET) {
            const struct sockaddr_in* ifa_addr = reinterpret_cast<const struct sockaddr_in*>(ifa->ifa_addr);
            if (memcmp(&addr.sin_addr, &ifa_addr->sin_addr, sizeof(addr.sin_addr)) == 0) {
                freeifaddrs(ifaddr);
                return true;
            }
        }
    }

    freeifaddrs(ifaddr);
    return false;
}

} // end anonymous namespace


std::string address::to_string() {
    sockaddr_in current_sockaddr = {};
    std::memcpy(&current_sockaddr, this, sizeof(sockaddr));

    char sender_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &current_sockaddr.sin_addr, sender_ip, INET_ADDRSTRLEN);

    return sender_ip;
}


network::network(uint16_t port):
    port_(port),
    receiving_sock_(create_receiving_socket(port)),
    broadcast_sock_(create_broadcast_socket()),
    peer2peer_sock_(create_peer2peer_socket()){
}

bool network::send(buffer message, address target_addr) {
    ssize_t sent_length = sendto(peer2peer_sock_, message.data, message.size, 0,
                                 (struct sockaddr *) &target_addr, sizeof(target_addr));
    if (sent_length < 0) {
        perror("Error sending message");
        return false;
    }

    return true;
}

bool network::broadcast(buffer message) {
    sockaddr_in broadcasting_address =  {
        .sin_family = AF_INET,
        .sin_port = htons(port_),
        .sin_addr = { .s_addr = inet_addr(BROADCAST_IP) }
    };

    if (sendto(broadcast_sock_, message.data, message.size, 0, (struct sockaddr *) &broadcasting_address, sizeof(broadcasting_address)) < 0) {
        perror("Error sending broadcast message");
        return false;
    }

    return true;
}

bool network::receive(buffer out_message, address *out_sender_addr) {
    std::memset(out_message.data, 0, out_message.size);

    sockaddr_in sender_address;
    socklen_t address_length = sizeof(sender_address);

    int received_length = recvfrom(receiving_sock_, out_message.data, out_message.size, 0, (struct sockaddr *) &sender_address, &address_length);
    if (received_length < 0) {
        // TODO: check if this error or async
        return false;
    }

    if (is_mine_address(sender_address)) {
        return false; // this is mine message
    }

    std::memcpy(out_sender_addr, &sender_address, sizeof(sockaddr));

    return true;
}

network::~network() {
    close(receiving_sock_);
    close(broadcast_sock_);
    close(peer2peer_sock_);
}
