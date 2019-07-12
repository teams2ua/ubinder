# Key-Value storage example

Simple library that simulate key-value store, each call to it takes 1s

If you want to build examples you will need to install [stlab](http://stlab.cc/) and [protobuf](https://developers.google.com/protocol-buffers/)
One of easy way to do this is with [vcpkg](https://github.com/Microsoft/vcpkg), then
```
vcpkg install stlab
vcpkg install protobuf
```

Then configuration will be like
```
cd ubinder_root
cmake -G"your build system"  -DCMAKE_TOOLCHAIN_FILE="path_to_vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake" -DWITH_EXAMPLES=1 ..
cmake --build . --config Release
```