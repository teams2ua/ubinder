#pragma once
#include "ubinder/function_types.h"

#ifdef WIN32
#  ifdef BUILDING_BINDING
        /* We are building binding that import functions from wrapper */
#    define WRAPPER_EXPORT __declspec(dllimport)
#  else
        /* We are not builind wrapper, so probably we are building wrapper */
#    define WRAPPER_EXPORT __declspec(dllexport)
#  endif
#else
#  define WRAPPER_EXPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif

WRAPPER_EXPORT void initWrapper(
    RequestResponse sendRequest,
    RequestResponse sendResponse,
    Notification sendNotification,
    RequestResponse* onRequest,
    RequestResponse* onResponse,
    Notification* onNotification);
    
#ifdef __cplusplus
}
#endif