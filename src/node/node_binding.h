#pragma once

#include "loops_tasks_queue.h"

#include <vector>

#include "../core/channel.h"
#include "../core/endpoint.h"

namespace ubinder {

class NodeBinding {
public:
    NodeBinding(uv_loop_t* uv_loop);
    void SendRequest(std::vector<uint8_t>&& reqData, Callback onResponse);
    void SendNotification(std::vector<uint8_t>&& notificationData);
private:
    QueuedTasks _tasksToQueue;
    Channel _channel;
    std::unique_ptr<Endpoint> server;
    std::unique_ptr<Endpoint> client;
};

}