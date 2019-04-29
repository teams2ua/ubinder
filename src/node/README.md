Assume you implement wrapper library for library you want to bind. Now you can create a bindings.

To create a node module:
1. Install nan https://www.npmjs.com/package/nan
2. Include ubinder.gypi in your gyp file.
3. Define variable target_module_name (name of the node module), wrapper_libs (full path to .lib/.so/.dylib of wrapper)
