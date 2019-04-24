project(wrapped_key_value_storage)

find_package(protobuf CONFIG REQUIRED)
find_package(stlab CONFIG REQUIRED)

set(PATH_TO_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/../proto)
file(GLOB ProtoFiles "${PATH_TO_PROTO}/*.proto")

add_library(wrapped_key_value_storage SHARED wrapped_lib.cpp)

protobuf_generate(
    LANGUAGE CPP
    TARGET wrapped_key_value_storage
    APPEND_PATH ${PATH_TO_PROTO}
    PROTOS ${ProtoFiles})

target_link_libraries(wrapped_key_value_storage PUBLIC key_value_storage)
target_link_libraries(wrapped_key_value_storage PRIVATE 
    protobuf::libprotoc
    protobuf::libprotobuf
    stlab::coroutines)

target_include_directories(wrapped_key_value_storage PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(wrapped_key_value_storage PRIVATE ${CMAKE_CURRENT_BINARY_DIR})