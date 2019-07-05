#include "java_binding.h"
#include "ubinder/wrapper_interface.h"
#include <vector>
#include <string>
//#include <iostream>
#include <memory>

#include "core/binding.h"

using namespace std;

ubinder::Binding ubinder::Binding::binding(initWrapper);

struct BinderJavaClass {
    JavaVM* vm;
    jobject jubinderGlobalRef;
    jmethodID onRequest;
    jmethodID onResponse;
    jmethodID onNotification;
};

BinderJavaClass javaClass;
std::unique_ptr<std::once_flag> initJavaObjectFlag;

JNIEnv* GetEnv() {
    JNIEnv* env;
    auto res = javaClass.vm->GetEnv((void**)& env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED) {
#ifdef ANDROID
        res = javaClass.vm->AttachCurrentThread(&env, NULL);
#else
        res = javaClass.vm->AttachCurrentThread((void**)&env, NULL);
#endif // ANDROID
    }
    if (res != JNI_OK) {
        //cout << "Can't get JNIEnv on message consuming thread" << endl;
        return nullptr;
    }
    return env;
}

void initJavaObject(JNIEnv* env, jobject obj) {
    // lambda in case of execution will be executed in the same execution context,
    // so it is safe to capture obj and env 
    std::call_once(*initJavaObjectFlag, [env, obj]() {
        javaClass.jubinderGlobalRef = env->NewGlobalRef(obj);
        ubinder::Binding::binding.Register(
            [](uint32_t request, vector<uint8_t>&& data) {
                JNIEnv* env = GetEnv();
                if (env == nullptr) {
                    return;
                }
                jbyteArray arr = env->NewByteArray(data.size());
                if (arr == NULL) {
                    //cout << "Can't create ByteArray" << endl;
                    return;
                }
                env->SetByteArrayRegion(arr, 0, data.size(), (jbyte*)data.data());
                env->CallVoidMethod(javaClass.jubinderGlobalRef, javaClass.onRequest, request, arr);
            },
            [](uint32_t request, vector<uint8_t>&& data) {
                JNIEnv* env = GetEnv();
                if (env == nullptr) {
                    return;
                }
                jbyteArray arr = env->NewByteArray(data.size());
                if (arr == NULL) {
                    //cout << "Can't create ByteArray" << endl;
                    return;
                }
                env->SetByteArrayRegion(arr, 0, data.size(), (jbyte*)data.data());
                env->CallVoidMethod(javaClass.jubinderGlobalRef, javaClass.onResponse, request, arr);
            },
            [](vector<uint8_t>&& data) {
                JNIEnv* env = GetEnv();
                if (env == nullptr) {
                    return;
                }
                jbyteArray arr = env->NewByteArray(data.size());
                if (arr == NULL) {
                    //cout << "Can't create ByteArray" << endl;
                    return;
                }
                env->SetByteArrayRegion(arr, 0, data.size(), (jbyte*)data.data());
                env->CallVoidMethod(javaClass.jubinderGlobalRef, javaClass.onNotification, arr);
            });
        ubinder::Binding::binding.StartListen();
    });
}



JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_ubinderInit(JNIEnv* env, jobject obj) {
    if (env->GetJavaVM(&javaClass.vm) < 0) {
        //cout << "Can't get pointer to JavaVM" << endl;
        return;
    }
    auto localHandler = env->FindClass("com/ubinder/Ubinder"); 
    if (localHandler == 0) {
        //cout << "Can't find class com.ubinder.Ubinder" << endl;
        return;
    }
    javaClass.onRequest = env->GetMethodID(localHandler, "OnRequest", "(I[B)V");
    if (javaClass.onRequest == 0) {
        //cout << "Can't find method OnRequest with signature (J[B)V" << endl;
        return;
    }
    javaClass.onResponse = env->GetMethodID(localHandler, "OnResponse", "(I[B)V");
    if (javaClass.onRequest == 0) {
        //cout << "Can't find method OnResponse with signature (J[B)V" << endl;
        return;
    }
    javaClass.onNotification = env->GetMethodID(localHandler, "OnNotification", "([B)V");
    if (javaClass.onNotification == 0) {
        //cout << "Can't find method OnNotification with signature ([B)V" << endl;
        return;
    }
    initJavaObjectFlag = std::make_unique<std::once_flag>();
}

JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendRequest(JNIEnv* env, jobject obj, jint req, jbyteArray arr) {
    initJavaObject(env, obj);
    vector<uint8_t> buffer(env->GetArrayLength(arr));
    env->GetByteArrayRegion(arr, 0, buffer.size(), (jbyte*)buffer.data());
    ubinder::Binding::binding.SendRequest((uint32_t)req, std::move(buffer));
}

JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendResponse(JNIEnv* env, jobject obj, jint req, jbyteArray arr) {
    initJavaObject(env, obj);
    vector<uint8_t> buffer(env->GetArrayLength(arr));
    env->GetByteArrayRegion(arr, 0, buffer.size(), (jbyte*)buffer.data());
    ubinder::Binding::binding.SendResponse((uint32_t)req, std::move(buffer));
}

JNIEXPORT void JNICALL Java_com_ubinder_Ubinder_SendNotification(JNIEnv* env, jobject obj, jbyteArray arr) {
    initJavaObject(env, obj);
    vector<uint8_t> buffer(env->GetArrayLength(arr));
    env->GetByteArrayRegion(arr, 0, buffer.size(), (jbyte*)buffer.data());
    ubinder::Binding::binding.SendNotification(std::move(buffer));
}
