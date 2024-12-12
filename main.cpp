#include "broadcast.h"
#include "log.h"

#include <chrono>

constexpr int PORT = 12345;
constexpr double WAIT_TIME = 1.0;

int main() {
    network net(12345);

    char send[256] = "discovery";

    while (true) {
        net.broadcast(send);
        LOG("Sending discovery message");

        auto start = std::chrono::high_resolution_clock::now();
        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - start;

            char recv[256] = {};
            net.receive(recv);

            if (elapsed.count() >= WAIT_TIME)
                break;
        }
    }

}
