To create a wrapper create a library and expose function from [include/ubinder/wrapper_interface.h](https://github.com/teams2ua/ubinder/blob/master/include/ubinder/wrapper_interface.h)

``` C
// [in] sendRequest - function that wrapper could call to send request to host, response will be received with call onResponse
// [in] sendResponse - function that wrapper should call when it respond for incoming request received in onRequest
// [in] sendNotification - function that wrapper could call when it want to send some notification to host
// [out] onRequest - function that host will call to send request to wrapper, response should be send by calling sendResponse
// [out] onResponse - function that host will call when it has a response for wrapper request sent by sendRequest
// [out] onNotification - function that host could call to send non-acknowledge notification

WRAPPER_EXPORT void initWrapper(
    RequestResponse sendRequest, 
    RequestResponse sendResponse,
    Notification sendNotification,
    RequestResponse* onRequest,
    RequestResponse* onResponse,
    Notification* onNotification);
```

C++ implementation is provided inside `include/cpp_wrapper.hpp`

After you have build wrapper library, follow instruction inside `src/{platform}` for building binding for specific host. (ex. `src/node` include instruction how to build Node.js binding)

## Building

Ubinder has no dependency
```shell
mkdir build
cd ./build
cmake -G"put here you build system" ..
cmake --build . --config Release
```