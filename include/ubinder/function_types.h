#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typeded void (*Callback)(char* data, uint32_t dataSize); 

typeded void (*Request)(char* data, uint32_t dataSize, Callback callback); 

#ifdef __cplusplus
}
#endif