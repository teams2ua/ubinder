#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Notification)(const char* data, size_t dataSize);

typedef void (*RequestResponse)(uint64_t request, const char* data, size_t dataSize);

#ifdef __cplusplus
}
#endif