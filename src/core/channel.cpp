#include "channel.h"

namespace ubinder {

std::unique_ptr<Endpoint> Channel::createServer(OnRequest onRequest, Callback onResponse) {
    return std::make_unique<Endpoint>(
            [&](const Message&& msg) {_serverToClient.push(std::move(msg)); },
            [&]() { return _clientToServer.get(); },
            onRequest,
            onResponse);
}

std::unique_ptr<Endpoint> Channel::createClient(OnRequest onRequest, Callback onResponse) {
    return std::make_unique<Endpoint>(
        [&](const Message&& msg) {_clientToServer.push(std::move(msg)); },
        [&]() { return _serverToClient.get(); },
        onRequest,
        onResponse);
}

}