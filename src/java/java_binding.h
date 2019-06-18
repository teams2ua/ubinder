#pragma once

#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

    JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendRequest(JNIEnv* env, jobject obj, jint req, jbyteArray arr);
    JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendResponse(JNIEnv* env, jobject obj, jint req, jbyteArray arr);
    JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendNotification(JNIEnv* env, jobject obj, jbyteArray arr);
    JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_ubinderInit(JNIEnv* env, jobject obj);

#ifdef __cplusplus
}
#endif