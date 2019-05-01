#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Notification)(const char* data, size_t dataSize);

// request should be the same that was passed in request
typedef void (*RequestResponse)(const void* request, const char* data, size_t dataSize);

#ifdef __cplusplus
}
#endif