#pragma once
#include "message_pipe.h"
#include "endpoint.h"

namespace ubinder {

class Channel {
public:
    std::unique_ptr<Endpoint> createServer(OnRequest onRequest, Callback onResponse);
    std::unique_ptr<Endpoint> createClient(OnRequest onRequest, Callback onResponse);
private:
    MessagePipe _serverToClient;
    MessagePipe _clientToServer;
};

};