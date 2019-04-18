cmake_minimum_required (VERSION 3.1)

project(ubinder CXX)

add_library(transport STATIC)
target_sources(transport PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/transport/message_pipe.cpp
    ${CMAKE_SOURCE_DIR}/src/transport/message_pipe.h
    ${CMAKE_SOURCE_DIR}/src/transport/transport_lib.cpp
    ${CMAKE_SOURCE_DIR}/src/transport/transport_lib.h)

target_include_directories(transport PUBLIC ${CMAKE_SOURCE_DIR}/include)


add_executable(test main.cpp)
target_link_libraries(test transport)