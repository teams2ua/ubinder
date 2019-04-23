#include "channel.h"

namespace ubinder {

void Channel::registerServerHandlers(OnRequest onRequest, OnResponse onResponse) {
    registerEndpoint(_server, _serverToClient, _clientToServer, onRequest, onResponse);
}

void Channel::registerClientHandlers(OnRequest onRequest, OnResponse onResponse) {
    registerEndpoint(_client, _clientToServer, _serverToClient, onRequest, onResponse);
}

void Channel::registerEndpoint(
    std::unique_ptr<Endpoint>& endpoint,
    MessagePipe& toSend,
    MessagePipe& toReceive,
    OnRequest onRequest,
    OnResponse onResponse) {
    std::lock_guard<std::mutex> lock(_registerLock);
    if (endpoint) {
        throw std::runtime_error("Endpoint is already registered");
    }
    endpoint = std::make_unique<Endpoint>(toSend, toReceive, onRequest, onResponse);
}

}