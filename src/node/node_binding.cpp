#include "../core/binding.h"
#include "ubinder/wrapper_interface.h"
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include <nan.h>
#include <v8.h>
#include <nan_typedarray_contents.h>
#include <uv.h>

#include "loops_tasks_queue.h"

using namespace ubinder;

std::unique_ptr<ubinder::QueuedTasks> tasksToQueue;
Binding ubinder::Binding::binding(initWrapper);

NAN_METHOD(sendRequest) {
    uint32_t reqId = Nan::To<v8::Uint32>(info[0]).ToLocalChecked()->Uint32Value();
    Nan::TypedArrayContents<uint8_t> buff(info[1]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    ubinder::Binding::binding.SendRequest(reqId, std::move(cpp));
}

NAN_METHOD(sendResponse) {
    uint32_t reqId = Nan::To<v8::Uint32>(info[0]).ToLocalChecked()->Uint32Value();
    Nan::TypedArrayContents<uint8_t> buff(info[1]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    ubinder::Binding::binding.SendResponse(reqId, std::move(cpp));
}

NAN_METHOD(sendNotification) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    ubinder::Binding::binding.SendNotification(std::move(data));
}

std::function<void()> CreateOnReqRespFunction(std::shared_ptr<Nan::Callback> onReqResp, uint32_t reqId, std::vector<uint8_t>&& data) {
    return
        [onReqResp, reqId, data{ std::move(data) }]() {
            Nan::HandleScope scope;
            v8::Local<v8::Number> serializedReqId = Nan::New(reqId);
            v8::Local<v8::Value> argv[] = {
                serializedReqId,
                Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
            };
            onReqResp->Call(2, argv);
        };
}

std::function<void()> CreateOnNotification(std::shared_ptr<Nan::Callback> onNotification, std::vector<uint8_t>&& data) {
    return
        [onNotification, data{ std::move(data) }]() {
        Nan::HandleScope scope;
        v8::Local<v8::Value> argv[] = {
            Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
        };
        onNotification->Call(1, argv);
    };
}

NAN_METHOD(registerLib) {
    auto onRequest = std::make_shared<Nan::Callback>(info[0].As<v8::Function>());
    auto onResponse = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    auto onNotification = std::make_shared<Nan::Callback>(info[2].As<v8::Function>());
    ubinder::Binding::binding.Register(
        [onRequest](uint32_t reqId, std::vector<uint8_t>&& data) { tasksToQueue->PushTask(CreateOnReqRespFunction(onRequest, reqId, std::move(data))); },
        [onResponse](uint32_t reqId, std::vector<uint8_t>&& data) { tasksToQueue->PushTask(CreateOnReqRespFunction(onResponse, reqId, std::move(data))); },
        [onNotification](std::vector<uint8_t>&& data) {tasksToQueue->PushTask(CreateOnNotification(onNotification, std::move(data)));}
    );
    ubinder::Binding::binding.StartListen();
}

NAN_MODULE_INIT(Init) {
    tasksToQueue = std::make_unique<ubinder::QueuedTasks>(uv_default_loop());
    NAN_EXPORT(target, sendRequest);
    NAN_EXPORT(target, sendResponse);
    NAN_EXPORT(target, sendNotification);
    NAN_EXPORT(target, registerLib);
}

NODE_MODULE(hello, Init)
