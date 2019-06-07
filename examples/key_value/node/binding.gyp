{
  "variables": {
    "target_module_name" : "key_store_node",
    "conditions": [
        ['OS=="win"', {"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/Debug/wrapped_lib.lib"}],
        ['OS=="linux"',{"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/wrapped_lib.so"}]
    ],
  },
  "includes": ["../../../src/node/ubinder.gypi"],
}
