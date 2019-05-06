{
  "variables": {
    "target_module_name" : "key_store_node",
    "conditions": [
        ['OS=="win"', {"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/Debug/wrapped_key_value_storage.lib"}],
        ['OS=="linux"',{"wrapper_libs": "../../../../build/examples/key_value/wrapped_lib/libwrapped_key_value_storage.so"}]
    ],
  },
  "includes": ["../../../src/node/ubinder.gypi"],
}
