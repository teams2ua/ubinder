#include "endpoint.h"

namespace ubinder {

Endpoint::Endpoint(MessagePipe& pipeToSendTo, MessagePipe& pipeToReceiveFrom)
    : _pipeToSendTo(pipeToSendTo)
    , _pipeToReceiveFrom(pipeToReceiveFrom) {}

void Endpoint::SendNotification(std::vector<uint8_t>&& notificationData) {
    Message messageToSend;
    messageToSend.type = NOTIFICATION;
    messageToSend.data = notificationData;
    messageToSend.req_id = -1;
    _pipeToSendTo.push(std::move(messageToSend));
}

void Endpoint::SendRequest(std::vector<uint8_t>&& requestData, Callback callback) {
    Message messageToSend;
    messageToSend.type = REQUEST;
    messageToSend.data = requestData;
    messageToSend.req_id = nextReqId.fetch_add(1);
    {
        std::lock_guard<std::mutex> guard(_pendingRequestsGuard);
        _pendingRequests[messageToSend.req_id] = callback;
    }
    _pipeToSendTo.push(std::move(messageToSend));
}

}