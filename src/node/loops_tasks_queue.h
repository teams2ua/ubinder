#pragma once

#include <functional>
#include <mutex>
#include <vector>

#include <uv.h>

namespace ubinder {

class QueuedTasks {
public:
    QueuedTasks(uv_loop_t* loop);
    void static call_on_loop(uv_async_t* handle);
    void CallOnLoop();
    void PushTask(const std::function<void()>&& func);
    ~QueuedTasks();
private:
    uv_async_t _async;
    std::mutex _lock;
    std::vector<std::function<void()>> _callbacks;
};

} //namespace ubinder