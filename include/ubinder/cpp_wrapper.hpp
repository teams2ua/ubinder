#pragma once
#include "ubinder/function_types.h"
#include <unordered_map>
#include <mutex>


namespace ubinder {

template<typename WrapperClass>
class CppWrapper {
public:
    typedef std::function<void(std::vector<uint8_t>&&)> Callback;
    typedef std::function<void(std::vector<uint8_t>&&, Callback&&)> OnRequest;
public:
    void SendRequest(std::vector<uint8_t>&& data, Callback&& callback) {
        uint32_t r;
        {
            std::lock_guard<std::mutex> lockGuard(_mapLock);
            r = ++reqId;
            _pendingCallbacks.emplace(r, std::move(callback));
        }
        
        sendRequest(r, (const char*)data.data(), data.size());
    }

    void SendNotification(std::vector<uint8_t>&& data) {
        sendNotification((const char*)data.data(), data.size());
    }

    //We receive incoming request
    void onRequest(uint32_t request, const char* data, size_t dataSize) {
        // we take the ownership of the data, so we copy
        std::vector<uint8_t> incomingData(data, data + dataSize);
        wrapper.OnRequest(std::move(incomingData), [request, this](std::vector<uint8_t>&& responseData) {
            sendResponse(request, (const char*)responseData.data(), responseData.size());
            });
    }

    //We receive answer for request that we sent in the past
    void onResponse(uint32_t request, const char* data, size_t dataSize) {
        Callback callback;
        {
            std::lock_guard<std::mutex> lockGuard(_mapLock);
            auto it = _pendingCallbacks.find(request);
            if (it == _pendingCallbacks.end())
                return;
            callback = std::move(it->second);
            _pendingCallbacks.erase(it);
        }
        std::vector<uint8_t> incomingData(data, data + dataSize);
        callback(std::move(incomingData));
    }

    //We receive notification
    void onNotification(const char* data, size_t dataSize) {
        std::vector<uint8_t> incomingData(data, data + dataSize);
        wrapper.OnNotification(std::move(incomingData));
    }

    void onExit() {
        wrapper.OnExit();
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
    ::ExitRequestResponse sendExit;
private:
    uint32_t reqId;
    std::mutex _mapLock;
    std::unordered_map<uint32_t, Callback> _pendingCallbacks;
};

}