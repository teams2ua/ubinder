#pragma once

#include <atomic>
#include <functional>
#include <thread>
#include <unordered_map>

#include "message_pipe.h"

namespace ubinder {

typedef std::function<void(std::vector<uint8_t>&& responseData)> OnResponse;
typedef std::function<void(uint32_t requestId, std::vector<uint8_t>&& requestData)> OnRequest;
typedef std::function<void(std::vector<uint8_t>&& notificationData)> OnNotification;

class Endpoint {
public:
    Endpoint(MessagePipe& pipeToSendTo, MessagePipe& pipeToReceiveFrom, OnRequest onRequest, OnNotification onNotification);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    void SendRequest(std::vector<uint8_t>&& requestData, OnResponse callback);
    ~Endpoint();
private:
    void loop();
private:
    MessagePipe& _pipeToSendTo;
    MessagePipe& _pipeToReceiveFrom;
    OnRequest _onRequest;
    OnNotification _onNotification;
    // request that for whom we didn't receive any response yet
    std::unordered_map<uint32_t, OnResponse> _pendingRequests;
    std::mutex _pendingRequestsGuard;
    std::thread _loopThread;
    std::atomic<uint32_t> _nextReqId;
};

}