#pragma once
#include "message_pipe.h"
#include "endpoint.h"

namespace ubinder {

struct Channel {
    MessagePipe _serverToClient;
    MessagePipe _clientToServer;
};

};