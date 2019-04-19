#pragma once
#include "message_pipe.h"
#include "endpoint.h"

namespace ubinder {

class Channel {
public:
    Channel();
private:
    MessagePipe _serverToClient;
    MessagePipe _clientToServer;
    Endpoint _server;
    Endpoint _client;
};

};