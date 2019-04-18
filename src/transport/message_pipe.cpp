#include "message_pipe.h"

namespace ubinder {

void MessagePipe::push(std::vector<uint8_t>&& message) {
    {
        std::lock_guard<std::mutex> lock(_lock);
        _queue.emplace(message);
    }
    _cv.notify_all();
}

std::vector<uint8_t> MessagePipe::get() {
    std::unique_lock<std::mutex> lock(_lock);
    _cv.wait(lock, [this] { return _queue.size();});
    auto message = std::move(_queue.front());
    _queue.pop();
    lock.unlock();
    return message;
}

}