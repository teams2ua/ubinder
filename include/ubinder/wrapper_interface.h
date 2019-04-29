#pragma once
#include "function_types.h"

// This file is counterpart of ubinder/wrapper_interface.h
// the usages of this two files are known beforehand, and interface should be stable
// so I decide to not bother with conditioning dllimport/dllexport

#ifdef WIN32
#  ifdef BUILDING_WRAPPER
        /* We are building this library */
#    define WRAPPER_EXPORT __declspec(dllexport)
#  else
        /* We are using this library */
#    define WRAPPER_EXPORT __declspec(dllimport)
#  endif
#else
#  define WRAPPER_EXPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif

WRAPPER_EXPORT void initWrapper(Request sendRequest, Callback sendNotification, Request* onRequest, Callback* onNotification);
    
#ifdef __cplusplus
}
#endif