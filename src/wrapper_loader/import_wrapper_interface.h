#pragma once
#include "function_types.h"

// This file is counterpart of ubinder/wrapper_interface.h
// the usages of this two files are known beforehand, and interface should be stable
// so I decide to not bother with conditioning dllimport/dllexport

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllimport) void initWrapper(Request sendRequest, Callback sendNotification, Request* onRequest, Callback* onNotification);
    
#ifdef __cplusplus
}
#endif