#include <vector>
#include <functional>

#include "stlab/concurrency/default_executor.hpp"
#include "stlab/concurrency/future.hpp"
#include "ubinder/wrapper_interface.h"
#include "ubinder/function_types.h"
#include "key_value_store.h"
#include "messages.pb.h"

class KeyValueStorageLib;

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
            this->onResponse(request, (const char*)responseData.data(), responseData.size());
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
protected:
    WrapperClass wrapper;
public:
    //functions to call
    ::Request sendRequest;
    ::Response sendResponse;
    ::Notification sendNotification;
};

static CppWrapper<KeyValueStorageLib> CppWrapperInstance;

void OnRequestFunc(const void* request, const char* data, size_t dataSize) {
    CppWrapperInstance.onRequest(request, data, dataSize);
}

void OnResponseFunc(const void* request, const char* data, size_t dataSize) {
    CppWrapperInstance.onResponse(request, data, dataSize);
}

void OnNotificationFunc(const char* data, size_t dataSize) {
    CppWrapperInstance.onNotification(data, dataSize);
}

using namespace key_value_protoc;

class KeyValueStorageLib {
public:
    std::string OnAddValue(const AddValueRequest& req) {
        AddValueResponse resp;
        try {
            storage.add(req.key(), req.value());
        }
        catch (std::exception& ex) {
            resp.set_error(ex.what());
        }
        return resp.SerializeAsString();
    }

    std::string OnGetValue(const GetValueRequest& req) {
        GetValueResponse resp;
        try {
            resp.set_value(storage.get(req.key()));
        }
        catch (std::exception& ex) {
            resp.set_error(ex.what());
        }
        return resp.SerializeAsString();
    }

    void OnRequest(std::vector<uint8_t>&& data, std::function<void(std::vector<uint8_t>&&)> && callback) {
        stlab::async(stlab::default_executor, [this, buffer{ std::move(data) }, cb(callback)]{
            key_value_protoc::Request req;
            if (!req.ParseFromArray(buffer.data(), buffer.size())) {
                throw std::runtime_error("Can't parse incoming messsage");
            }
            switch (req.request_type()) {
            case ADD_VALUE: {
                AddValueRequest addValReq;
                addValReq.ParseFromString(req.request_body());
                return OnAddValue(addValReq);
            }
            case GET_VALUE: {
                GetValueRequest getValReq;
                getValReq.ParseFromString(req.request_body());
                return OnGetValue(getValReq);
            }
            default:
                throw std::runtime_error("Unknown message type");
            };
            }).recover([cb(callback)](const stlab::future<std::string>& x) {
                try {
                    const std::string value = *x.get_try();
                    std::vector<uint8_t> v((uint8_t*)value.data(), (uint8_t*)value.data() +value.size());
                    cb(std::move(v));
                }
                catch (const std::exception& ex) {
                    std::cout << "some error" << std::endl;
                }
                });
    }
public:
    key_value_store::Storage storage;
};


extern "C" {
    void initWrapper(
        ::Request sendRequest,
        ::Response sendResponse,
        ::Notification sendNotification,
        ::Request* onRequest,
        ::Response* onResponse,
        ::Notification* onNotification) {
        CppWrapperInstance.sendRequest = sendRequest;
        CppWrapperInstance.sendResponse = sendResponse;
        CppWrapperInstance.sendNotification = sendNotification;
        *onRequest = OnRequestFunc;
        *onResponse = OnResponseFunc;
        *onNotification = OnNotificationFunc;
    }
}
