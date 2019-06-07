#include "core/binding.h"
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


std::unique_ptr<ubinder::QueuedTasks> tasksToQueue;
Binding Binding::binding(initWrapper);

NAN_METHOD(sendRequest) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> cpp(*buff, *buff + buff.length());
    auto callback = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::Callback lmb([callback](std::vector<uint8_t>&& data){
        tasksToQueue->PushTask([callback{ std::move(callback) }, data{std::move(data)}]{
            Nan::HandleScope scope;
            v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
            callback->Call(1, argv);
            });
    });
    ubinder::Binding::binding.SendRequest(std::move(cpp), std::move(lmb));
}

NAN_METHOD(sendNotification) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    ubinder::Binding::binding.SendNotification(std::move(data));
}

void OnResponseInNode(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    Nan::TypedArrayContents<uint8_t> pointerData(info[1]);
    uint64_t pointer;
    memcpy(&pointer, *pointerData, 8);
    ubinder::Callback* cb = reinterpret_cast<ubinder::Callback*>(pointer);
    (*cb)(std::move(data));
    delete cb;
}


std::function<void()> CreateOnRequestFunction(std::shared_ptr<Nan::Callback> jsOnRequest, std::vector<uint8_t>&& data, ubinder::Callback&& callback) {
    return
        [jsOnRequest, data{ std::move(data) }, callback{ std::move(callback) }]() {
            Nan::HandleScope scope;
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(OnResponseInNode);
            auto onResponseInNode = tpl->GetFunction();
            auto cb = new ubinder::Callback(std::move(callback));
            std::vector<uint8_t> serializedPointer(8);
            memcpy(&serializedPointer[0], &cb, 8);
            v8::Local<v8::Value> argv[] = {
                Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
                onResponseInNode,
                Nan::CopyBuffer((char*)serializedPointer.data(), serializedPointer.size()).ToLocalChecked()
            };
            jsOnRequest->Call(3, argv);
        };
}

NAN_METHOD(registerLib) {
    auto onRequest = std::make_shared<Nan::Callback>(info[0].As<v8::Function>());
    auto onNotification = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::Binding::binding.RegisterServer(
        [onRequest](std::vector<uint8_t>&& data, ubinder::Callback&& callback) {
            tasksToQueue->PushTask(CreateOnRequestFunction(onRequest, std::move(data), std::move(callback)));
        }
       ,
        [onNotification](std::vector<uint8_t>&& data) {
            Nan::HandleScope scope;
            v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
            onNotification->Call(1, argv);
        }
    );
    ubinder::Binding::binding.StartListen();
}

NAN_MODULE_INIT(Init) {
    tasksToQueue = std::make_unique<ubinder::QueuedTasks>(uv_default_loop());
    NAN_EXPORT(target, sendRequest);
    NAN_EXPORT(target, sendNotification);
    NAN_EXPORT(target, registerLib);
}

NODE_MODULE(hello, Init)
