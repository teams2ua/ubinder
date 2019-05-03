#pragma once

#include "ubinder/wrapper_interface.h"

#include <vector>

#include "../core/channel.h"
#include "../core/endpoint.h"

namespace ubinder {

class NodeBinding {
public:
    NodeBinding();
    void SendRequest(std::vector<uint8_t>&& reqData, Callback&& onResponse);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    // Next 3 functions will be called by wrapper lib to send some data to binding
    static void onRequestFromWrapper(const void* request, const char* data, size_t dataSize);
    static void onResponseFromWrapper(const void* request, const char* data, size_t dataSize);
    static void onNotificationFromWrapper(const char* data, size_t dataSize);

    // Pointer to the function that client is listening to
    // We should call this when we have something for client
    ::RequestResponse _clientOnRequest;
    ::RequestResponse _clientOnResponse;
    ::Notification _clientOnNotification;

    void RegisterServer(std::function<void(std::vector<uint8_t>&&, Callback&&)>&& onRequest, Callback&& onNotification);

    static ubinder::NodeBinding nodeBinding;
private:
    Channel _channel;
    std::unique_ptr<Endpoint> _server;
    std::unique_ptr<Endpoint> _client;

};

}