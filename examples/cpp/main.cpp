#include <thread>
#include <iostream>
#include <vector>

#include "binder.hpp"

void server(binder::MessagePipe& client_server) {
    for (int i = 0; i < 11; ++i) {
        std::cout << (uint8_t)client_server.get()[2] << std::endl;
    }
}

void clientProcessRequest(std::vector<uint8_t>&& request, std::function<void(std::vector<uint8_t>&&)> callback) {
    
}

void clientProcessNotification(std::vector<uint8_t>&& notification) {
    
}

int main() {
    binder::library lib;
    auto client = lib.getClient(clientProcessRequest, clientProcessNotification);
    auto server = lib.getServer(serverProcessRequest, serverProcessNotification);
    std::thread server_thread([&]() {server(client_server); });
    for (int i = 0; i < 11; ++i) {
        int val;
        std::cin >> val;
        client_server.push({ 0x00, 0x01, static_cast<uint8_t>(val) });
    }
    server_thread.join();
    return 0;
}