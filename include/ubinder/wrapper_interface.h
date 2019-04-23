#pragma once
#include "ubinder/function_types.h"

// This is the interface wrapper should provide,
// ubinder will call initLibrary and pass two his function sendRequest and sendNotificaiton
// that should be used by library to send information to other side,
// library should return two its own function that should be called on request and 
// on notification.

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void initWrapper(Request sendRequest, Callback sendNotification, Request* onRequest, Callback* onNotification);
    
#ifdef __cplusplus
}
#endif