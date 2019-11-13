#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
#include "include/libavcodec/avcodec.h"


}

#define LOG_TAG "cxwFFmpeg"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_alick_ffmpeglearn2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpeglearn2_MainActivity_parseFile(JNIEnv *env, jobject thiz, jstring file_path) {

    std::string hello = "11111111111";

    LOGI("哈哈哈哈");

    avcodec_register_all();




    LOGE("休息休息");
    return env->NewStringUTF(hello.c_str());

}