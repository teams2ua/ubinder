#pragma once

#include <queue>
#include <vector>
#include <thread>
#include <mutex>

namespace ubinder {

class MessagePipe {
public:
    void push(std::vector<uint8_t>&& message);
    std::vector<uint8_t> get();
private:
    std::queue<std::vector<uint8_t>> _queue;
    std::mutex _lock;
    std::condition_variable _cv;
};

}