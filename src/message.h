#pragma once

#include <vector>

namespace ubinder {

enum MESSAGE_TYPE {
    NOTIFICATION,
    REQUEST,
    RESPONSE
};

struct Message {
    std::vector<uint8_t> data;
    uint64_t req_id;
    MESSAGE_TYPE type;
};

}