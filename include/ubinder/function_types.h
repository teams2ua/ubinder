#pragma once
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Notification)(const char* data, size_t dataSize);

typedef void (*RequestResponse)(uint32_t request, const char* data, size_t dataSize);

typedef void (*ExitRequestResponse)();

#ifdef __cplusplus
}
#endif
