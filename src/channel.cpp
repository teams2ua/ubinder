#include "channel.h"

namespace ubinder {

Channel::Channel()
    : _server(_serverToClient, _clientToServer)
    , _client(_clientToServer, _serverToClient) {
}

}