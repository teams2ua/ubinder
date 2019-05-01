#pragma once
#include "function_types.h"

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

WRAPPER_EXPORT void initWrapper(
    Request sendRequest,
    Response sendResponse,
    Notification sendNotification,
    Request* onRequest,
    Response* onResponse,
    Notification* onNotification);
    
#ifdef __cplusplus
}
#endif