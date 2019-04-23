#pragma once
#include <mutex>
#include <memory>

#include "message_pipe.h"
#include "endpoint.h"

namespace ubinder {

class Channel {
public:
    void registerServerHandlers(OnRequest onRequest, OnResponse onResponse);
    void registerClientHandlers(OnRequest onRequest, OnResponse onResponse);
private:
    void registerEndpoint(
        std::unique_ptr<Endpoint>& endpoint,
        MessagePipe& toSend,
        MessagePipe& toReceive,
        OnRequest onRequest,
        OnResponse onResponse);
private:
    MessagePipe _serverToClient;
    MessagePipe _clientToServer;
    std::unique_ptr<Endpoint> _server;
    std::unique_ptr<Endpoint> _client;
    std::mutex _registerLock;
};

};