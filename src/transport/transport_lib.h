#pragma once

#include <string>
#include <unordered_map>

#include "message_pipe.h"

namespace ubinder {

class TransportLib {
public:
    
private:
    std::unordered_map<std::string, MessagePipe> _pipes;
};

}