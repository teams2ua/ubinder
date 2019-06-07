#include <vector>
#include <functional>

#include "stlab/concurrency/default_executor.hpp"
#include "stlab/concurrency/immediate_executor.hpp"
#include "stlab/concurrency/future.hpp"
#include "ubinder/wrapper_interface.h"
#include "ubinder/function_types.h"
#include "ubinder/cpp_wrapper.hpp"

#include "key_value_store.h"
#include "messages.pb.h"

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
                }).detach();
    }

    void OnNotification(std::vector<uint8_t>&& data) {
        std::cout << "some notification" << std::endl;
    }
public:
    key_value_store::Storage storage;
};


static ubinder::CppWrapper<KeyValueStorageLib> CppWrapperInstance;

void OnRequestFunc(uint64_t request, const char* data, size_t dataSize) {
    CppWrapperInstance.onRequest(request, data, dataSize);
}

void OnResponseFunc(uint64_t request, const char* data, size_t dataSize) {
    CppWrapperInstance.onResponse(request, data, dataSize);
}

void OnNotificationFunc(const char* data, size_t dataSize) {
    CppWrapperInstance.onNotification(data, dataSize);
}


extern "C" {
    void initWrapper(
        ::RequestResponse sendRequest,
        ::RequestResponse sendResponse,
        ::Notification sendNotification,
        ::RequestResponse* onRequest,
        ::RequestResponse* onResponse,
        ::Notification* onNotification) {
        CppWrapperInstance.sendRequest = sendRequest;
        CppWrapperInstance.sendResponse = sendResponse;
        CppWrapperInstance.sendNotification = sendNotification;
        *onRequest = &OnRequestFunc;
        *onResponse = &OnResponseFunc;
        *onNotification = &OnNotificationFunc;
    }
}
