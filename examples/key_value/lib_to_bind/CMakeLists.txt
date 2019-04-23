project(key_value_storage)

add_library(key_value_storage STATIC key_value_store.cpp key_value_store.h)
target_include_directories(key_value_storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})