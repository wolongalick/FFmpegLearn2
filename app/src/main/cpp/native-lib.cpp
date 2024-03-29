#include <jni.h>
#include <string>
#include <android/log.h>
#include <queue>


extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
#include "cxw_str.h"
}

#define LOG_TAG "cxwFFmpeg"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);


static double r2d(AVRational avRational) {
    LOGI("num:%d,den:%d", avRational.num, avRational.den);
    return avRational.num == 0 || avRational.den == 0 ? 0 : (double) avRational.num /
                                                            (double) avRational.den;
}

/**
 * 获取当前毫秒
 * @return
 */
long long getNowMs() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    LOGI("tv_sec:%ld,tv_usec:%ld", tv.tv_sec, tv.tv_usec);
    int sec = tv.tv_sec;//100小时
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    av_jni_set_java_vm(vm, nullptr);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpeglearn2_MainActivity_parseFile(JNIEnv *env, jobject thiz, jstring file_path) {

    std::string hello = "11111111111";


    av_register_all();

    avformat_network_init();

    avcodec_register_all();

    std::string config = avcodec_configuration();
    LOGI("ffmpeg配置%s", config.c_str());

    const char *path = env->GetStringUTFChars(file_path, nullptr);
    LOGI("文件路径:%s", path)

    AVFormatContext *ic = nullptr;

    int re = avformat_open_input(&ic, path, nullptr, nullptr);

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
            videoStream = i;
            LOGI("视频流索引:%d", i);
            fps = r2d(pStream->avg_frame_rate);
            LOGI("fps:%d,宽:%d,高:%d,解码器:%d,pixel_format:%d", fps, pStream->codecpar->width,
                 pStream->codecpar->height, pStream->codecpar->codec_id, pStream->codecpar->format);
        } else if (pStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStream = i;
            LOGI("音频流索引:%d", i);
            LOGI("sample_rate:%d,channels:%d,sample_format:%d", pStream->codecpar->sample_rate,
                 pStream->codecpar->channels, pStream->codecpar->format);
        }
    }

    //
    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    LOGI("视频流索引:%d", videoStream);
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    LOGI("音频流索引:%d", audioStream);


    /*==========================视频解码-begin=============================================================*/
    //软解码器
    AVCodec *pVideoAVCodec = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id);
    //硬解码器
//    pVideoAVCodec = avcodec_find_decoder_by_name("h264_mediacodec");

    if (!pVideoAVCodec) {
        LOGE("查找视频解码器失败!");
        return env->NewStringUTF("查找视频解码器失败!");
    }

    LOGI("查找视频解码器成功");

    //视频解码器初始化
    AVCodecContext *vc = avcodec_alloc_context3(pVideoAVCodec);
    avcodec_parameters_to_context(vc, ic->streams[videoStream]->codecpar);
    vc->thread_count = 8;
    //打开视频解码器
    re = avcodec_open2(vc, nullptr, nullptr);
    if (re != 0) {
        char *err = mergeStr(2, "打开视频解码器失败:", av_err2str(re));
        LOGE("%s", err);
        return env->NewStringUTF(err);
    }

    LOGI("打开视频解码器成功!");
    /*===========================视频解码-end============================================================*/
    /*===========================音频解码-begin============================================================*/

    //软解码器
    AVCodec *pAudioAVCodec = avcodec_find_decoder(ic->streams[audioStream]->codecpar->codec_id);
    //硬解码器
//    pVideoAVCodec=avcodec_find_decoder_by_name("h254_mediacodec");

    if (!pAudioAVCodec) {
        LOGE("查找音频解码器失败!");
        return env->NewStringUTF("查找音频解码器失败!");
    }

    LOGI("查找音频解码器成功");

    //音频解码器初始化
    AVCodecContext *ac = avcodec_alloc_context3(pAudioAVCodec);
    avcodec_parameters_to_context(ac, ic->streams[audioStream]->codecpar);
    ac->thread_count = 1;
    //打开音频解码器
    re = avcodec_open2(ac, nullptr, nullptr);
    if (re != 0) {
        char *err = mergeStr(2, "打开音频解码器失败:", av_err2str(re));
        LOGE("%s", err);
        return env->NewStringUTF(err);
    }

    LOGI("打开音频解码器成功!");

    /*===========================音频解码-end============================================================*/



    //读取帧数据
    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pAVFrame = av_frame_alloc();

    long long start = getNowMs();
    int frameCount = 0;

    for (;;) {
        //每隔三秒统计一次fps(每秒解析多少帧)
        if (getNowMs() - start >= 3000) {
            LOGI("当前解码fps是 %d", frameCount / 3);
            start = getNowMs();
            LOGI("毫秒数,start:%lld", start);
            frameCount = 0;
        }

        int re = av_read_frame(ic, pPacket);
        if (re != 0) {
            LOGI("读取到结尾处!");
            int pos = 20 * r2d(ic->streams[videoStream]->time_base);
            av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
            continue;
        }

        //非视频情况,直接continue
        /*if(pPacket->stream_index!=videoStream){
            continue;
        }*/

//        LOGI("stream=%d,size=%d,pts=%lld,flag=%d",pPacket->stream_index,pPacket->size,pPacket->pts,pPacket->flags);



        AVCodecContext *cc = vc;
        if (pPacket->stream_index == audioStream) {
            cc = ac;
        }

        //发送到线程中解码
        re = avcodec_send_packet(cc, pPacket);
        int p = pPacket->pts;
        //清理
        av_packet_unref(pPacket);
        if (re != 0) {
            LOGE("avcodec_send_packet失败,原因:%s", av_err2str(re));
            continue;
        }


        for (;;) {
            re = avcodec_receive_frame(cc, pAVFrame);
            if (re != 0) {
                LOGE("avcodec_receive_frame失败,原因:%s", av_err2str(re));
                break;
            }
            LOGI("avcodec_receive_frame %lld", pAVFrame->pts);

            if (cc == vc) {
                //如果是视频帧
                frameCount++;
            }
        }
    }


    avformat_close_input(&ic);

    return env->NewStringUTF(hello.c_str());

}