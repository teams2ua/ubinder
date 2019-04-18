#include <thread>
#include <iostream>
#include <vector>

#include "MessagePipe.hpp"

void server(binder::MessagePipe& client_server) {
    for (int i = 0; i < 11; ++i) {
        std::cout << (uint8_t)client_server.get()[2] << std::endl;
    }
}

int main() {
    binder::MessagePipe client_server;
    std::thread server_thread([&]() {server(client_server); });
    for (int i = 0; i < 11; ++i) {
        int val;
        std::cin >> val;
        client_server.push({ 0x00, 0x01, static_cast<uint8_t>(val) });
    }
    server_thread.join();
    return 0;
}