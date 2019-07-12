# Ubinder for Node

## Installation

```
yarn add ubinder
yarn add nan
```
Create `binding.gyp` file, include ubinder.gypi and define two variables that it use
    * `target_module_name` - this will be the .node file you building
    * `wrapper_libs` - absolut path to the native binary that implement ubinder-wrapper interface (see https://raw.githubusercontent.com/teams2ua/ubinder/master/README.md)
For example:

```
{
  "variables": {
    "target_module_name" : "key_store_node",
    "conditions": [
        ['OS=="win"', {"wrapper_libs": "<(module_root_dir)/libs/win/wrapped_lib.lib"}],
        ['OS=="linux"',{"wrapper_libs": "<(module_root_dir)/libs/linux/wrapped_lib.so"}]
    ],
  },
  "includes": ["node_modules/ubinder/src/node/ubinder.gypi"],
}
```

Then build your native module
```
yarn
```

Now you have Node binding for your native library, that has async interface.
Simple, right? 
