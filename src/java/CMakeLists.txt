add_library(ubinder_jni SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/java_binding.h
    ${CMAKE_CURRENT_SOURCE_DIR}/java_binding.cpp)

if (NOT ANDROID)
    find_package(JNI REQUIRED)
endif()


target_include_directories(ubinder_jni PUBLIC ${JNI_INCLUDE_DIRS})
target_link_libraries(ubinder_jni PUBLIC ${JNI_LIBRARIES})
target_link_libraries(ubinder_jni PUBLIC ubinder_core)

find_library(WRAPPED_LIB ledger-core PATHS ${PATH_TO_WRAPPED_LIB})

target_link_libraries(ubinder_jni PUBLIC ${WRAPPED_LIB}) # wrapped_lib is provided by client

target_include_directories(ubinder_jni PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})