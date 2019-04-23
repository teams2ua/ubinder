#include "endpoint.h"

namespace ubinder {

Endpoint::Endpoint(
    MessagePipe& pipeToSendTo,
    MessagePipe& pipeToReceiveFrom,
    OnRequest onRequest,
    OnNotification onNotification)
    : _pipeToSendTo(pipeToSendTo)
    , _pipeToReceiveFrom(pipeToReceiveFrom)
    , _onRequest(onRequest)
    , _onNotification(onNotification) {
    std::swap(_loopThread, std::thread([this]() {this->loop(); }));
}

void Endpoint::SendNotification(std::vector<uint8_t>&& notificationData) {
    Message messageToSend;
    messageToSend.type = NOTIFICATION;
    messageToSend.data = notificationData;
    messageToSend.req_id = -1;
    _pipeToSendTo.push(std::move(messageToSend));
}

void Endpoint::SendRequest(std::vector<uint8_t>&& requestData, OnResponse callback) {
    Message messageToSend;
    messageToSend.type = REQUEST;
    messageToSend.data = requestData;
    messageToSend.req_id = _nextReqId.fetch_add(1);
    {
        std::lock_guard<std::mutex> guard(_pendingRequestsGuard);
        _pendingRequests[messageToSend.req_id] = callback;
    }
    _pipeToSendTo.push(std::move(messageToSend));
}

void Endpoint::loop() {
    while (true) {
        Message msg = _pipeToReceiveFrom.get();
        switch (msg.type) {
            case EXIT:
                return;
            case NOTIFICATION:
                _onNotification(std::move(msg.data));
                break;
            case REQUEST:
                _onRequest(msg.req_id, std::move(msg.data));
                break;
            case RESPONSE: {
                OnResponse onResponse;
                {
                    std::lock_guard<std::mutex> guard(_pendingRequestsGuard);
                    auto it = _pendingRequests.find(msg.req_id);
                    if (it != _pendingRequests.end())
                        break;
                    std::swap(onResponse, it->second);
                    _pendingRequests.erase(msg.req_id);
                }
                onResponse(std::move(msg.data));
                break;
            }
                
        }
    }
}

Endpoint::~Endpoint() {
    try {
        // send exit to our own event loop
        Message exit_message;
        exit_message.type = EXIT;
        _pipeToReceiveFrom.push(std::move(exit_message));
        _loopThread.join();
    }
    catch (...) {// no exceptions should escape from destructor
    }
}

}