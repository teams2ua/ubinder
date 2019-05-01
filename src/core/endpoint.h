#pragma once

#include <functional>
#include <thread>

#include "message_pipe.h"

namespace ubinder {

typedef std::function<void(std::vector<uint8_t>&&)> Callback;
typedef std::function<void(void*, std::vector<uint8_t>&&)> RequestResponse;

typedef std::function<void(Message&&)> PushFunction;
typedef std::function<Message()> GetFunction;


class Endpoint {
public:
    Endpoint(PushFunction&& pushFunction, GetFunction&& getFunction, RequestResponse&& onRequest, RequestResponse&& onResponse, Callback&& onNotification);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    void SendRequest(void* request, std::vector<uint8_t>&& requestData);
    // request - same value as 
    void SendResponse(void* request, std::vector<uint8_t>&& responseData);
    ~Endpoint();
private:
    void loop();
private:
    PushFunction _pushFunction;
    GetFunction _getFunction;
    RequestResponse _onRequest;
    RequestResponse _onResponse;
    Callback _onNotification;
    std::thread _loopThread;
};

}