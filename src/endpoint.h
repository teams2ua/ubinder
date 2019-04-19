#pragma once

#include <atomic>
#include <functional>
#include <unordered_map>

#include "message_pipe.h"

namespace ubinder {

class Endpoint {
public:
    typedef std::function<void(std::vector<uint8_t>&& responseData)> Callback;
    Endpoint(MessagePipe& pipeToSendTo, MessagePipe& pipeToReceiveFrom);
    void SendNotification(std::vector<uint8_t>&& notificationData);
    void SendRequest(std::vector<uint8_t>&& requestData, Callback callback);
private:
    MessagePipe& _pipeToSendTo;
    MessagePipe& _pipeToReceiveFrom;
    // request that for whom we didn't receive any response yet
    std::unordered_map<uint32_t, Callback> _pendingRequests;
    std::mutex _pendingRequestsGuard;
    std::thread _loopThread;
    std::atomic<uint32_t> nextReqId;
};

}