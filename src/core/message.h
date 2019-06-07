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
    uint64_t request;
    MESSAGE_TYPE type;
};

}