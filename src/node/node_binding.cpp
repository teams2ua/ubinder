#include <nan.h>
#include <node.h>
#include <iostream>

using namespace v8;
using namespace node;

void sendNotification(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (info[0]->IsUint8Array()) {
    std::cout << "Is Uint8Array()" << std::endl;
  }
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("sendNotification").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(sendNotification)->GetFunction());
}

NODE_MODULE(node_wrapper, Init)

