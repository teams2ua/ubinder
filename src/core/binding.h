#pragma once

#include "ubinder/function_types.h"

#include <vector>

#include "channel.h"
#include "endpoint.h"

typedef void (*InitWrapperFunction)(
    RequestResponse sendRequest,
    RequestResponse sendResponse,
    Notification sendNotification,
    RequestResponse* onRequest,
    RequestResponse* onResponse,
    Notification* onNotification);

namespace ubinder {

class Binding {
public:
    Binding(InitWrapperFunction initWrapper);
    void SendRequest(uint32_t request, std::vector<uint8_t>&& reqData);
    void SendResponse(uint32_t request, std::vector<uint8_t>&& reqData);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    // Next 3 functions will be called by wrapper lib to send some data to binding
    static void onRequestFromWrapper(uint32_t request, const char* data, size_t dataSize);
    static void onResponseFromWrapper(uint32_t request, const char* data, size_t dataSize);
    static void onNotificationFromWrapper(const char* data, size_t dataSize);

    // Pointer to the function that client is listening to
    // We should call this when we have something for client
    ::RequestResponse _clientOnRequest;
    ::RequestResponse _clientOnResponse;
    ::Notification _clientOnNotification;

    void Register(
        std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onRequest,
        std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onResponse,
        std::function<void(std::vector<uint8_t>&&)>&& onNotification);

    void StartListen();

    static ubinder::Binding binding;
private:
    Channel _channel;
    std::unique_ptr<Endpoint> _server;
    std::unique_ptr<Endpoint> _client;

};

}