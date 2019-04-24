#include <vector>

#include "stlab/concurrency/default_executor.hpp"
#include "stlab/concurrency/future.hpp"
#include "ubinder/wrapper_interface.h"
#include "ubinder/function_types.h"
#include "key_value_store.h"
#include "messages.pb.h"


static key_value_store::Storage storage;

using namespace key_value_protoc;

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

void OnRequest(char* data, size_t dataSize, Callback callback) {
    key_value_protoc::Request req;
    if (!req.ParseFromArray(data, dataSize)) {
        throw std::runtime_error("Can't parse incoming messsage");
    }
    stlab::async(stlab::default_executor, [req] {
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
    }).recover();
}

void OnNotification(char* data, size_t dataSize) {

}

void initWrapperCpp(::Request sendRequest, Callback sendNotification, ::Request* onRequest, Callback* onNotification) {
    *onRequest = &OnRequest;
    *onNotification = &OnNotification;
}

extern "C" {
    void initWrapper(::Request sendRequest, Callback sendNotification, ::Request* onRequest, Callback* onNotification) {
        initWrapperCpp(sendRequest, sendNotification, onRequest, onNotification);
    }
}
