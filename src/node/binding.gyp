{
  'variables': {
    'target_module_name%': 'node_wrapper',
    'ubinder_root_dir%': '../..',
    'wrapper_lib_name%': 'wrapper',
    'conditions': [
      ['OS=="win"', { 'lib_name': '<(wrapper_lib_name)' }],
      ['OS!="win"', { 'lib_name': 'lib<(wrapper_lib_name)' }],
    ]
  },
  'targets': [{
    'target_name': '<(target_module_name)',
    'sources': [ "<(ubinder_root_dir)/src/node/node_binding.cpp",
                 "<(ubinder_root_dir)/src/core/channel.cpp",
                 "<(ubinder_root_dir)/src/core/channel.h", 
                 "<(ubinder_root_dir)/src/core/endpoint.cpp",
                 "<(ubinder_root_dir)/src/core/endpoint.h",
                 "<(ubinder_root_dir)/src/core/message.h",
                 "<(ubinder_root_dir)/src/core/message_pipe.cpp",
                 "<(ubinder_root_dir)/src/core/message_pipe.h"],
    'include_dirs': [
      "<!(node -e \"require('nan')\")",
      "<(ubinder_root_dir)/include",
    ],
    'conditions': [
      ['OS=="mac"', {
        'LDFLAGS': [
          '-framework IOKit',
          '-framework CoreFoundation',
          '-DYLD_LIBRARY_PATH '
        ],
        'libraries': [
          '-l<(lib_name)'
        ],
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'OTHER_CFLAGS': [
            '-ObjC++',
            '-std=c++14',
          ],
          'OTHER_LDFLAGS': [
            '-framework IOKit',
            '-framework CoreFoundation',
            '-Xlinker -rpath -Xlinker @loader_path/'
          ],
        },
      }],
      ['OS=="win"', {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'OTHER_CFLAGS': ['-std=c++14'],
        'include_dirs': ['$(UCRT_BASED_PATH)'],
        'libraries': ['<(lib_name)'],
      }],
      ['OS=="linux"', {
        'ldflags': [
          '-Wl,-R,\'$$ORIGIN\'',
        ],
        'libraries': [
          '-l<(lib_name)',
        ],
      }],
    ],
    'cflags!': ['-ansi', '-fno-exceptions'],
    'cflags_cc!': ['-fno-exceptions'],
    'cflags': ['-g', '-exceptions'],
    'cflags_cc': ['-g', '-exceptions']
  }]
}