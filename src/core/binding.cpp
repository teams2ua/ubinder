#include "binding.h"

namespace ubinder {

    Binding::Binding(InitWrapperFunction initWrapper) {
        // connect wrapper interface initWrapper function to "client" side of channel (both side of channel are endpoints)
        // all the information is known on link time, so we can create client endpoint in constructor of static object
        // server side will be registered later in separete function
        initWrapper(&onRequestFromWrapper, &onResponseFromWrapper, &onNotificationFromWrapper, &_clientOnRequest, &_clientOnResponse, &_clientOnNotification);
        _client = std::make_unique<Endpoint>(
            [this](Message&& msg) {this->_channel._clientToServer.push(std::move(msg)); },
            [this]() {return this->_channel._serverToClient.get(); },
            [this](uint64_t request, std::vector<uint8_t>&& data) {this->_clientOnRequest(request, (const char*)data.data(), data.size()); },
            [this](uint64_t request, std::vector<uint8_t>&& data) {this->_clientOnResponse(request, (const char*)data.data(), data.size()); },
            [this](std::vector<uint8_t>&& data) {this->_clientOnNotification((const char*)data.data(), data.size()); });
    }

    void Binding::onRequestFromWrapper(uint64_t request, const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        binding._client->SendRequest(request, std::move(buffer));
    }

    void Binding::onResponseFromWrapper(uint64_t request, const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        binding._client->SendResponse(request, std::move(buffer));
    }
    
    void Binding::onNotificationFromWrapper(const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        binding._client->SendNotification(std::move(buffer));
    }
    ///////////////////////////////////

    void Binding::SendRequest(uint64_t request, std::vector<uint8_t>&& reqData) {
        _server->SendRequest(request, std::move(reqData));
    }

    void Binding::SendResponse(uint64_t request, std::vector<uint8_t>&& reqData) {
        _server->SendResponse(request, std::move(reqData));
    }

    void Binding::SendNotification(std::vector<uint8_t>&& reqData) {
        _server->SendNotification(std::move(reqData));
    }

    void Binding::Register(
        std::function<void(uint64_t, std::vector<uint8_t>&&)>&& onRequest,
        std::function<void(uint64_t, std::vector<uint8_t>&&)>&& onResponse,
        std::function<void(std::vector<uint8_t>&&)>&& onNotification) {
        _server = std::make_unique<Endpoint>(
            [this](Message&& msg) {this->_channel._serverToClient.push(std::move(msg)); },
            [this]() {return this->_channel._clientToServer.get(); },
            std::move(onRequest),
            std::move(onResponse),
            std::move(onNotification));
    }
    /////////////////////////////

    void Binding::StartListen() {
        _client->StartListen();
        _server->StartListen();
    }
};