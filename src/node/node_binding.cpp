#include "node_binding.h"
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include <nan.h>
#include <v8.h>
#include <nan_typedarray_contents.h>
#include <uv.h>

#include "loops_tasks_queue.h"

namespace ubinder {

    NodeBinding::NodeBinding(uv_loop_t* loop)
        : _tasksToQueue(loop) {
        initWrapper(&onRequestForWrapper, &onNotificationForWrapper, &_clientOnRequest, &_clientOnNotification);

        _client = _channel.createClient(
            [this](std::vector<uint8_t>&& requestData, Callback callback) {
                _clientOnRequest(this, (char*)requestData.data(), requestData.size(), );
            },
            );
    }

    void NodeBinding::SendRequest(std::vector<uint8_t>&& reqData, Callback&& onResponse) {
        // callback will be called from different thread, so we need to push a task on event loop
        auto& queue = _tasksToQueue;
        _server->SendRequest(std::move(reqData), std::move([onResp{std::move(onResponse)}, &queue](std::vector<uint8_t>&& data){
            queue.PushTask([d{ std::move(data) }, onResponse{std::move(onResp)}]() mutable { onResponse(std::move(d)); });
        }));
    }

    void NodeBinding::SendNotification(std::vector<uint8_t>&& reqData) {
        _server->SendNotification(std::forward<std::vector<uint8_t>>(reqData));
    }

    void NodeBinding::RegisterServer(OnRequest&& onRequest, Callback&& onNotification) {
        _server = _channel.createServer(onRequest, onNotification);
    }
};

static std::unique_ptr<ubinder::NodeBinding> nodeBinding;

NAN_METHOD(getLength){
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::cout << buff.length() << std::endl;
}

NAN_METHOD(thereAndBack) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    info.GetReturnValue().Set(Nan::CopyBuffer((char*)cpp.data(), cpp.size()).ToLocalChecked());
}

NAN_METHOD(Method) {
    std::vector<uint8_t> some_data {0,1,2,3,4,5,6,7,8,9,10};
    info.GetReturnValue().Set(Nan::CopyBuffer((char*)some_data.data(), some_data.size()).ToLocalChecked());
}

NAN_METHOD(sendRequest) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    auto callback = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::Callback lmb([callback](std::vector<uint8_t>&& data){
        Nan::HandleScope scope;
        v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
        callback->Call(1, argv);
    });
    nodeBinding->SendRequest(std::move(cpp), std::move(lmb));
}

NAN_METHOD(sendNotification) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    nodeBinding->SendNotification(std::move(data));
}

NAN_METHOD(registerLib) {
    auto onRequest = std::make_shared<Nan::Callback>(info[0].As<v8::Function>());
    auto onNotification = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::OnRequest lmbRequest([onRequest](std::vector<uint8_t> && data, Callback onResponse) {
        /* TODO implement me
        Nan::HandleScope scope;
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(responseCallback);
        v8::Local<v8::Function> fn = tpl->GetFunction();
        v8::Local<v8::Value> argv[] = {
            Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
            fn
        };
        onRequest->Call(2, argv);
        /**/
        });
    ubinder::OnRequest lmbNotification([onRequest](std::vector<uint8_t>&& data) {
        });
    nodeBinding->RegisterServer(std::move(lmbRequest), std::move(lmbNotification));
}

NAN_MODULE_INIT(Init) {
    nodeBinding = std::make_unique<ubinder::NodeBinding>(uv_default_loop());
    NAN_EXPORT(target, Method);
    NAN_EXPORT(target, getLength);
    NAN_EXPORT(target, thereAndBack);
    NAN_EXPORT(target, sendRequest);
    NAN_EXPORT(target, sendNotification);
}

NODE_MODULE(hello, Init)
