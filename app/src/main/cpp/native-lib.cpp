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


static double r2d(AVRational avRational) {
    return avRational.num == 0 || avRational.den == 0 ? 0 : (double) avRational.num /
                                                            (double) avRational.den;
}

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

    int fps = 0;
    int width = 0;
    int height = 0;
    int videoStream = 0;
    int audioStream = 1;

    for (int i = 0; i < ic->nb_streams; ++i) {
        AVStream *pStream = ic->streams[i];
        if (pStream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            LOGI("视频流索引:%d", i);
            fps=r2d(pStream->avg_frame_rate);
            LOGI("fps:%d,宽:%d,高:%d,解码器:%d,pixel_format:%d",fps,pStream->codecpar->width,pStream->codecpar->height,pStream->codecpar->codec_id,pStream->codecpar->format);
        } else if (pStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStream=i;
            LOGI("音频流索引:%d", i);
            LOGI("sample_rate:%d,channels:%d,sample_format:%d",pStream->codecpar->sample_rate,pStream->codecpar->channels,pStream->codecpar->format);
        }
    }

    //
    videoStream= av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    LOGI("视频流索引:%d", videoStream);
    audioStream= av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    LOGI("音频流索引:%d", audioStream);


//    av_q2d(ic->video_codec.);









    avformat_close_input(&ic);

    return env->NewStringUTF(hello.c_str());

}