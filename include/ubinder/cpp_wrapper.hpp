#pragma once
#include "ubinder/function_types.h"

namespace ubinder {

template<typename WrapperClass>
class CppWrapper {
public:
    typedef std::function<void(std::vector<uint8_t>&&)> Callback;
    typedef std::function<void(std::vector<uint8_t>&&, Callback&&)> OnRequest;
public:
    void SendRequest(std::vector<uint8_t>&& data, Callback&& callback) {
        sendRequest(new Callback(std::move(callback)), (const char*)data.data(), data.size());
    }

    void SendNotification(std::vector<uint8_t>&& data) {
        sendNotification((const char*)data.data(), data.size());
    }

    //We receive incoming request
    void onRequest(const void* request, const char* data, size_t dataSize) {
        // we take the ownership of the data, so we copy
        std::vector<uint8_t> incomingData(data, data + dataSize);
        wrapper.OnRequest(std::move(incomingData), [request, this](std::vector<uint8_t>&& responseData) {
            sendResponse(request, (const char*)responseData.data(), responseData.size());
            });
    }

    //We receive answer for request that we sent in the past
    void onResponse(const void* request, const char* data, size_t dataSize) {
        const Callback* callback = reinterpret_cast<const Callback*>(request);
        std::vector<uint8_t> incomingData(data, data + dataSize);
        (*callback)(std::move(incomingData));
        delete callback;
    }

    //We receive notification
    void onNotification(const char* data, size_t dataSize) {
        std::vector<uint8_t> incomingData(data, data + dataSize);
        wrapper.OnNotification(std::move(incomingData));
    }

    void initWrapper() {
        wrapper.Init();
    }
protected:
    WrapperClass wrapper;
public:
    //functions to call
    ::RequestResponse sendRequest;
    ::RequestResponse sendResponse;
    ::Notification sendNotification;
};

}