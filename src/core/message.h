#pragma once

#include <vector>

namespace ubinder {

enum MESSAGE_TYPE {
    NOTIFICATION,
    REQUEST,
    RESPONSE,
    EXIT
};

struct Message {
    std::vector<uint8_t> data;
    const void* request { nullptr };
    MESSAGE_TYPE type;
};

}