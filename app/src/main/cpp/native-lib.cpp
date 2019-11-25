#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>

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


char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpeglearn2_MainActivity_parseFile(JNIEnv *env, jobject thiz, jstring file_path) {

    std::string hello = "配置";
    hello+=avcodec_configuration();

    LOGI("ffmpeg配置:%s",hello.c_str());

    //初始化解封装
    av_register_all();
    //初始化网路
    avformat_network_init();

    AVFormatContext *ic=NULL;
    char* path=jstringToChar(env,file_path);
    //打开媒体文件
    int re = avformat_open_input(&ic, path, 0, 0);
    if(re==0){
        LOGI("打开文件成功,文件路径:%s ",path)
    } else{
        LOGE("打开文件失败:%s",av_err2str(re))
    }


//
//    AVStream
//    AVPacket

//
//    avformat_find_stream_info()
//
//    av_find_best_stream()
//
//    av_read_frame()


    LOGE("休息休息");
    return env->NewStringUTF(hello.c_str());

}