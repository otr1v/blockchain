#include "broadcast.h"
#include "blockchain.h"

constexpr int PORT = 12345;

int main() {
    LOG("INIT: program started");

    network net(PORT);
    blockchain chain(0, std::move(net));
    chain.run();
}
