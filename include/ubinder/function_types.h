#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Callback)(char* data, size_t dataSize); 

typedef void (*Request)(char* data, size_t dataSize, Callback callback);

#ifdef __cplusplus
}
#endif