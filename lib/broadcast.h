#pragma once

#include <cstdint>
#include <string>


struct buffer {
    void *data;
    std::size_t size;

    buffer(void *data, std::size_t size):
        data(data),
        size(size) {
    }

    template <typename type, std::size_t size>
    buffer(type array[size]):
        buffer(array, size * sizeof(type)) {
    }

    template <typename type>
    buffer(type &element):
        buffer(&element, sizeof(type)) {
    }
};

// Opaque storage for address of a node (ip + port)
struct address {
    char data[16];

    std::string to_string();
};

class network {
public:
    network(uint16_t port);

    bool send(buffer message, address target);
    bool broadcast(buffer message);
    bool receive(buffer out_message, address *out_sender_addr);

    network(const network &other) = delete;
    network(const network &&other):
        port_(other.port_),
        broadcast_sock_(other.broadcast_sock_),
        receiving_sock_(other.receiving_sock_),
        peer2peer_sock_(other.peer2peer_sock_) {
    }

    ~network();

private:
    uint16_t port_;

    int broadcast_sock_;
    int receiving_sock_;
    int peer2peer_sock_;
};
