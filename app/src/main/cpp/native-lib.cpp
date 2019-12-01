#include <jni.h>
#include <string>
#include <android/log.h>
#include <queue>


extern "C" {
#include <libavformat/avformat.h>
#include "cxw_str.h"
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

    av_register_all();

    std::string config = avcodec_configuration();
    LOGI("ffmpeg配置%s", config.c_str());

    const char *path = env->GetStringUTFChars(file_path, 0);
    LOGI("文件路径:%s", path)

    AVFormatContext *ic = NULL;

    int re = avformat_open_input(&ic, path, 0, 0);

    if (re != 0) {
        char *string = mergeStr(2, "打开文件失败,原因:", av_err2str(re));
        jstring pJstring = env->NewStringUTF(string);
        free(string);
        return pJstring;
    }

    LOGI("打开文件成功");



    LOGI("文件时长:%lld秒,文件流个数%d", ic->duration / AV_TIME_BASE, ic->nb_streams);


    avformat_close_input(&ic);

    LOGE("休息休息");
    return env->NewStringUTF(hello.c_str());

}