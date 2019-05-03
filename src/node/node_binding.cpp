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

    NodeBinding::NodeBinding() {
        // connect wrapper interface initWrapper function to "client" side of channel (both side of channel are endpoints)
        // all the information is known on link time, so we can create client endpoint in constructor of static object
        // server side will be registered later in separete function
        initWrapper(&onRequestFromWrapper, &onResponseFromWrapper, &onNotificationFromWrapper, &_clientOnRequest, &_clientOnResponse, &_clientOnNotification);
        _client = std::make_unique<Endpoint>(
            [this](Message&& msg) {this->_channel._clientToServer.push(std::move(msg)); },
            [this]() {return this->_channel._serverToClient.get(); },
            [this](const void* request, std::vector<uint8_t>&& data) {this->_clientOnRequest(request, (const char*)data.data(), data.size()); },
            [this](const void* request, std::vector<uint8_t>&& data) {this->_clientOnResponse(request, (const char*)data.data(), data.size()); },
            [this](std::vector<uint8_t>&& data) {this->_clientOnNotification((const char*)data.data(), data.size()); });
    }

    void NodeBinding::onRequestFromWrapper(const void* request, const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        nodeBinding._client->SendRequest(request, std::move(buffer));
    }

    void NodeBinding::onResponseFromWrapper(const void* request, const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        nodeBinding._client->SendResponse(request, std::move(buffer));
    }
    
    void NodeBinding::onNotificationFromWrapper(const char* data, size_t dataSize) {
        std::vector<uint8_t> buffer(data, data + dataSize);
        nodeBinding._client->SendNotification(std::move(buffer));
    }
    ///////////////////////////////////

    void NodeBinding::SendRequest(std::vector<uint8_t>&& reqData, Callback&& onResponse) {
        Callback* cb = new Callback(std::move(onResponse));
        _server->SendRequest(reinterpret_cast<const void*>(cb), std::move(reqData));
    }

    void NodeBinding::SendNotification(std::vector<uint8_t>&& reqData) {
        _server->SendNotification(std::forward<std::vector<uint8_t>>(reqData));
    }

    void NodeBinding::RegisterServer(std::function<void(std::vector<uint8_t>&&, Callback&&)> && onRequest, Callback&& onNotification) {
        _server = std::make_unique<Endpoint>(
            [this](Message&& msg) {this->_channel._serverToClient.push(std::move(msg)); },
            [this]() {return this->_channel._clientToServer.get(); },
            [this, onReq{std::move(onRequest)}](const void* request, std::vector<uint8_t>&& data) {
                onReq(std::move(data), [this, request](std::vector<uint8_t>&& buffer) {
                    this->_server->SendResponse(request, std::move(buffer));
                    });
            },
            [](const void* request, std::vector<uint8_t>&& data) {
                const Callback* cb = reinterpret_cast<const Callback*>(request);
                (*cb)(std::move(data));
                delete cb;
            },
            [onNotif{std::move(onNotification)}](std::vector<uint8_t>&& data) {
                onNotif(std::move(data));
            });
    }

    NodeBinding NodeBinding::nodeBinding;
};

std::unique_ptr<ubinder::QueuedTasks> tasksToQueue;

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
        tasksToQueue->PushTask([callback{ std::move(callback) }, data{std::move(data)}]{
            Nan::HandleScope scope;
            v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
            callback->Call(1, argv);
            });
    });
    ubinder::NodeBinding::nodeBinding.SendRequest(std::move(cpp), std::move(lmb));
}

NAN_METHOD(sendNotification) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    ubinder::NodeBinding::nodeBinding.SendNotification(std::move(data));
}

void OnResponseInNode(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    Nan::TypedArrayContents<uint8_t> buff(info[0]);
    std::vector<uint8_t> data(*buff, *buff + buff.length());
    Nan::TypedArrayContents<uint8_t> pointerData(info.Callee()->GetInternalField(0));
    uint64_t pointer;
    memcpy(&pointer, *pointerData, 8);
    ubinder::Callback* cb = reinterpret_cast<ubinder::Callback*>(pointer);
    (*cb)(std::move(data));
    delete cb;
}

NAN_METHOD(registerLib) {
    auto onRequest = std::make_shared<Nan::Callback>(info[0].As<v8::Function>());
    auto onNotification = std::make_shared<Nan::Callback>(info[1].As<v8::Function>());
    ubinder::NodeBinding::nodeBinding.RegisterServer(
        [onRequest](std::vector<uint8_t>&&data, ubinder::Callback&& callback) {
            Nan::HandleScope scope;
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(OnResponseInNode);
            v8::Local<v8::Function> onResponseInNode = tpl->GetFunction();
            auto cb = new ubinder::Callback(std::move(callback));
            std::vector<uint8_t> serializedPointer(8);
            memcpy(&serializedPointer[0], &cb, 8);
            onResponseInNode->SetInternalField(0, Nan::CopyBuffer((char*)serializedPointer.data(), serializedPointer.size()).ToLocalChecked());
            v8::Local<v8::Value> argv[] = {
                Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked(),
                onResponseInNode
            };
            onRequest->Call(2, argv);
        },
        [onNotification](std::vector<uint8_t>&& data) {
            Nan::HandleScope scope;
            v8::Local<v8::Value> argv[] = { Nan::CopyBuffer((char*)data.data(), data.size()).ToLocalChecked() };
            onNotification->Call(1, argv);
        }
    );
}

NAN_MODULE_INIT(Init) {
    tasksToQueue = std::make_unique<ubinder::QueuedTasks>(uv_default_loop());
    NAN_EXPORT(target, sendRequest);
    NAN_EXPORT(target, sendNotification);
    NAN_EXPORT(target, registerLib);
}

NODE_MODULE(hello, Init)
