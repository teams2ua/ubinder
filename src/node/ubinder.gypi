{
  "targets": [
  {
    'target_name': '<(target_module_name)',
    'defines': ["BUILDING_BINDING"],
    'sources': [ "node_binding.cpp",
                 "loops_tasks_queue.cpp",
                 "loops_tasks_queue.h",
                 "../core/channel.h", 
                 "../core/endpoint.cpp",
                 "../core/endpoint.h",
                 "../core/message.h",
                 "../core/message_pipe.cpp",
                 "../core/message_pipe.h"],
    'include_dirs': [
      "<!(node -e \"require('nan')\")",
      "../../include",
    ],
    'libraries': ["<(wrapper_libs)"],
    'conditions': [
      ['OS=="mac"', {
        'xcode_settings': {
          'OTHER_CFLAGS': [
            '-ObjC++',
            '-std=c++14'
          ],
          'OTHER_LDFLAGS': [
            '-framework IOKit',
            '-framework CoreFoundation',
            '-Xlinker -rpath -Xlinker @loader_path/'
          ]
        }
      }],
      ['OS=="linux"', {'ldflags': ['-Wl,-R,\'$$ORIGIN\'']}],
      ['OS=="win"', {'cflags_cc': ['/EHsc']}],
    ],
    'cflags!': ['-ansi', '-fno-exceptions'],
    'cflags_cc!': ['-fno-exceptions'],
    'cflags': ['-g', '-exceptions'],
    'cflags_cc': ['-g', '-exceptions']
  }]
}