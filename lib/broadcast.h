#pragma once

#include <cstdint>


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
        buffer(element, sizeof(type)) {
    }
};

class network {
public:
    network(uint16_t port);

    bool broadcast(buffer message);
    bool receive(buffer out_message);

    ~network();

private:
    uint16_t port_;

    int broadcast_sock_;
    int receiving_sock_;
};
