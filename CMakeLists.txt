cmake_minimum_required (VERSION 3.1)

project(ubinder)

add_library(ubinder STATIC)
target_sources(ubinder PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/channel.h
    ${CMAKE_SOURCE_DIR}/src/core/channel.cpp
    ${CMAKE_SOURCE_DIR}/src/core/endpoint.h
    ${CMAKE_SOURCE_DIR}/src/core/endpoint.cpp
    ${CMAKE_SOURCE_DIR}/src/core/message.h
    ${CMAKE_SOURCE_DIR}/src/core/message_pipe.h
    ${CMAKE_SOURCE_DIR}/src/core/message_pipe.cpp)
target_include_directories(ubinder PUBLIC ${CMAKE_SOURCE_DIR}/include)


if (WITH_EXAMPLES)
    add_subdirectory(examples)
endif()