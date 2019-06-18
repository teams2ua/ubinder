#include "endpoint.h"

namespace ubinder {

Endpoint::Endpoint(
    PushFunction&& pushFunction,
    GetFunction&& getFunction,
    std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onRequest,
    std::function<void(uint32_t, std::vector<uint8_t>&&)>&& onResponse,
    std::function<void(std::vector<uint8_t>&&)>&& onNotification)
    : _pushFunction(std::move(pushFunction))
    , _getFunction(std::move(getFunction))
    , _onRequest(std::move(onRequest))
    , _onResponse(std::move(onResponse))
    , _onNotification(std::move(onNotification)) {
}

void Endpoint::SendNotification(std::vector<uint8_t>&& notificationData) {
    _pushFunction(std::move(Message{ std::move(notificationData), 0, NOTIFICATION }));
}

void Endpoint::SendRequest(uint32_t request, std::vector<uint8_t>&& requestData) {
    _pushFunction(std::move(Message{ std::move(requestData), request, REQUEST }));
}

void Endpoint::SendResponse(uint32_t request, std::vector<uint8_t>&& responseData) {
    _pushFunction(std::move(Message{ std::move(responseData), request, RESPONSE }));
}

void Endpoint::StartListen() {
    _loopThread = std::make_unique<std::thread>(([this]() {this->loop(); }));
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
                _onRequest(msg.request, std::move(msg.data));
                break;
            }
            case RESPONSE: {
                _onResponse(msg.request, std::move(msg.data));
                break;
            }
            default:
                return;
        }
    }
}

Endpoint::~Endpoint() {
    _loopThread->join();
}

}