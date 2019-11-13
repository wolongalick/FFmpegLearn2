package com.alick.ffmpeglearn2.utils;

import android.text.TextUtils;
import android.util.Log;


public class BLog {

    public static boolean IS_USE_GALAN_VIDEO;   //是否使用Galan的视频

    public static String TAG = "cxw_FFmpeg";

    /**
     * 日志总开关
     */
    public static boolean rootLogOn = true;

    public static final boolean DEBUGV = true;

    // public static final boolean DEBUGD = Log.isLoggable(CotaLog.TAG, Log.DEBUG);
    public static final boolean DEBUGD = true;

    public static final boolean DEBUGI = true;

    public static final boolean DEBUGW = true;

    public static void setTag(String tag) {
        TAG = tag;
    }

    public static void i(String msg) {
        i(TAG, msg);
    }

    public static void i(String tag, String msg) {
        if (rootLogOn && DEBUGI) {
            if (!TextUtils.isEmpty(tag)) {
                Log.i(tag, msg);
            } else {
                Log.i(TAG, msg);
            }
        }
    }

    public static void v(String msg) {
        v(TAG, msg);
    }

    public static void v(String tag, String msg) {
        if (rootLogOn && DEBUGI) {
            if (!TextUtils.isEmpty(tag)) {
                Log.v(tag, msg);
            } else {
                Log.v(TAG, msg);
            }
        }
    }

    public static void d(String msg) {
        d(TAG, msg);
    }

    public static void d(String tag, String msg) {
        if (rootLogOn && DEBUGI) {
            if (!TextUtils.isEmpty(tag)) {
                Log.d(tag, msg);
            } else {
                Log.d(TAG, msg);
            }
        }
    }

    public static void e(String msg) {
        e(TAG, msg);
    }

    public static void e(String tag, String msg) {
        if (rootLogOn && DEBUGI) {
            if (!TextUtils.isEmpty(tag)) {
                Log.e(tag, msg);
            } else {
                Log.e(TAG, msg);
            }
        }
    }

    public static void w(String msg) {
        w(TAG, msg);
    }

    public static void w(String tag, String msg) {
        if (rootLogOn && DEBUGI) {
            if (!TextUtils.isEmpty(tag)) {
                Log.w(tag, msg);
            } else {
                Log.w(TAG, msg);
            }
        }
    }
}
