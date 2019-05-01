#pragma once

#include "loops_tasks_queue.h"
#include "ubinder/wrapper_interface.h"

#include <vector>

#include "../core/channel.h"
#include "../core/endpoint.h"

namespace ubinder {

class NodeBinding {
public:
    NodeBinding(uv_loop_t* uv_loop);
    void SendRequest(std::vector<uint8_t>&& reqData, Callback&& onResponse);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    //void RegisterServer(OnRequest&& onRequest, Callback&& onNotification);
private:
    static void onRequestForWrapper(void* request, const char* data, size_t dataSize, ::Callback callback);
    static void onNotificationForWrapper(void* request, const char* data, size_t dataSize);
    // Pointer to the function that client is listening to
    // We should call this when we have something for client
    ::RequestResponse _clientOnRequest;
    ::RequestResponse _clientOnResponse;
    ::Notification _clientOnNotification;
private:
    QueuedTasks _tasksToQueue;
    Channel _channel;
    std::unique_ptr<Endpoint> _server;
    std::unique_ptr<Endpoint> _client;

};

}