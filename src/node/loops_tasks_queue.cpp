#pragma once

#include "loops_tasks_queue.h"

#include <functional>
#include <mutex>
#include <vector>

#include <uv.h>

namespace ubinder {

    QueuedTasks::QueuedTasks(uv_loop_t* loop) {
        _async.data = this;
        uv_async_init(loop, &_async, call_on_loop);
    }

    void static QueuedTasks::call_on_loop(uv_async_t* handle) {
        QueuedTasks* self = (QueuedTasks*)handle->data;
        self->CallOnLoop();
    }

    void QueuedTasks::CallOnLoop() {
        std::vector<std::function<void()>> cbs;
        {
            std::lock_guard<std::mutex> guard(_lock);
            cbs.swap(_callbacks);
        }
        for (auto cb : cbs) {
            cb();
        }
    }

    void QueuedTasks::PushTask(const std::function<void()>&& func) {
        {
            std::lock_guard<std::mutex> guard(_lock);
            _callbacks.emplace_back(func);
        }
        uv_async_send(&_async);
    }

    QueuedTasks::~QueuedTasks() {
        uv_close((uv_handle_t*)& _async, nullptr);
    }
} //namespace ubinder