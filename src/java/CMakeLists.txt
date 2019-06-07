add_library(ubinder_jni SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/java_binding.h
    ${CMAKE_CURRENT_SOURCE_DIR}/java_binding.cpp)

find_package(JNI REQUIRED)

target_include_directories(ubinder_jni PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(ubinder_jni PUBLIC ${JNI_LIBRARIES})
target_link_libraries(ubinder_jni PUBLIC ubinder_core)
target_link_libraries(ubinder_jni PUBLIC wrapped_lib) # wrapped_lib is provided by client

target_include_directories(ubinder_jni PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})