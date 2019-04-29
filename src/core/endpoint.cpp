#include "endpoint.h"

namespace ubinder {

Endpoint::Endpoint(
    PushFunction pushFunction,
    GetFunction getFunction,
    OnRequest onRequest,
    Callback onNotification)
    : _pushFunction(pushFunction)
    , _getFunction(getFunction)
    , _onRequest(onRequest)
    , _onNotification(onNotification) {
    std::swap(_loopThread, std::thread([this]() {this->loop(); }));
}

void Endpoint::SendNotification(std::vector<uint8_t>&& notificationData) {
    Message messageToSend;
    messageToSend.type = NOTIFICATION;
    messageToSend.data = notificationData;
    messageToSend.req_id = -1;
    _pushFunction(std::move(messageToSend));
}

void Endpoint::SendRequest(std::vector<uint8_t>&& requestData, Callback callback) {
    Message messageToSend;
    messageToSend.type = REQUEST;
    messageToSend.data = requestData;
    messageToSend.req_id = _nextReqId.fetch_add(1);
    {
        std::lock_guard<std::mutex> guard(_pendingRequestsGuard);
        _pendingRequests[messageToSend.req_id] = callback;
    }
    _pushFunction(std::move(messageToSend));
}

void Endpoint::loop() {
    while (true) {
        Message msg = _getFunction();
        switch (msg.type) {
            case EXIT:
                return;
            case NOTIFICATION:
                _onNotification(std::move(msg.data));
                break;
            case REQUEST: {
                uint64_t req_id = msg.req_id;
                _onRequest(std::move(msg.data), [&](std::vector<uint8_t>&& data) {
                    Message messageToSend;
                    messageToSend.type = RESPONSE;
                    messageToSend.data = data;
                    messageToSend.req_id = req_id;
                    _pushFunction(std::move(messageToSend));
                });
                break;
            }
            case RESPONSE: {
                Callback onResponse;
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
    _loopThread.join();
}

}