* `lib_to_bind` - original library that we want to bind
* `proto` - we define protocol to communicate with this `lib_to_bind`
* `wrapped_lib` - wrapper that implements ubinder exported symbols and translate commands defined in `proto` to `lib_to_bind` call
* `node` - place where we define .gyp file for node-addon and place symbols
* `java` - java binding