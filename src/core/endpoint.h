#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <unordered_map>

#include "message_pipe.h"

namespace ubinder {

typedef std::function<void(std::vector<uint8_t>&& data)> Callback;
typedef std::function<void(std::vector<uint8_t>&& requestData, Callback callback)> OnRequest;

typedef std::function<void(Message&& data)> PushFunction;
typedef std::function<Message()> GetFunction;


class Endpoint {
public:
    Endpoint(PushFunction pushFunction, GetFunction getFunction, OnRequest onRequest, Callback onNotification);
    Endpoint(Endpoint&&) = default;
    void SendNotification(std::vector<uint8_t>&& notificationData);
    void SendRequest(std::vector<uint8_t>&& requestData, Callback callback);
    ~Endpoint();
private:
    void loop();
private:
    PushFunction _pushFunction;
    GetFunction _getFunction;
    OnRequest _onRequest;
    Callback _onNotification;
    // request that for whom we didn't receive any response yet
    std::unordered_map<uint64_t, Callback> _pendingRequests;
    std::mutex _pendingRequestsGuard;
    std::thread _loopThread;
    std::atomic<uint32_t> _nextReqId;
};

}