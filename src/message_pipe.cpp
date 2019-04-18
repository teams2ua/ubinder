#include "message_pipe.h"

namespace ubinder {

void MessagePipe::push(Message&& message) {
    {
        std::lock_guard<std::mutex> lock(_lock);
        _queue.emplace(message);
    }
    _cv.notify_all();
}

Message MessagePipe::get() {
    std::unique_lock<std::mutex> lock(_lock);
    _cv.wait(lock, [this] { return _queue.size();});
    auto message = std::move(_queue.front());
    _queue.pop();
    lock.unlock();
    return message;
}

}