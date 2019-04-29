#pragma once

#include <queue>
#include <thread>
#include <mutex>

#include "message.h"

namespace ubinder {

class MessagePipe {
public:
    void push(const Message&& message);
    Message get();
private:
    std::queue<Message> _queue;
    std::mutex _lock;
    std::condition_variable _cv;
};

}