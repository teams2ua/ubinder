#pragma once

#include <functional>
#include <thread>

#include "message_pipe.h"

namespace ubinder {

typedef std::function<void(Message&&)> PushFunction;
typedef std::function<Message()> GetFunction;


class Endpoint {
public:
    Endpoint(
        PushFunction&& pushFunction,
        GetFunction&& getFunction,
        std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onRequest,
        std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onResponse,
        std::function<void(std::vector<uint8_t>&&)>&& onNotification);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    void SendRequest(uint32_t request, std::vector<uint8_t>&& requestData);
    // request - same value as 
    void SendResponse(uint32_t request, std::vector<uint8_t>&& responseData);
    void StartListen();
    ~Endpoint();
private:
    void loop();
private:
    PushFunction _pushFunction;
    GetFunction _getFunction;
    std::function<void(uint32_t, std::vector<uint8_t>&&)> _onRequest;
    std::function<void(uint32_t, std::vector<uint8_t>&&)> _onResponse;
    std::function<void(std::vector<uint8_t>&&)> _onNotification;
    std::unique_ptr<std::thread> _loopThread;
};

}