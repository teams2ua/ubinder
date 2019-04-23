project(wrapped_key_value_storage)

find_package(protobuf CONFIG REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/../proto/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

add_library(wrapped_key_value_storage SHARED wrapped_lib.cpp ${ProtoSources} ${ProtoHeaders})
target_link_libraries(wrapped_key_value_storage PUBLIC key_value_storage)
target_link_libraries(wrapped_key_value_storage PRIVATE protobuf::libprotoc protobuf::libprotobuf)

target_include_directories(wrapped_key_value_storage PUBLIC ${CMAKE_SOURCE_DIR}/include/ubinder)