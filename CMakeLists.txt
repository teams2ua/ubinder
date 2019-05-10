cmake_minimum_required (VERSION 3.1)

project(ubinder)

# wrapper could link this library to have include dirs
add_library(ubinder_wrapper INTERFACE)
target_include_directories(ubinder_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# new host implementation could link core library
# but depends on the build system used on the platform
# for example Node-addons prefer to use node-gyp
add_library(ubinder_core STATIC)
target_sources(ubinder_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/channel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/endpoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/endpoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/message.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/message_pipe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/message_pipe.cpp)
target_include_directories(ubinder_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


if (WITH_EXAMPLES)
    add_subdirectory(examples)
endif()