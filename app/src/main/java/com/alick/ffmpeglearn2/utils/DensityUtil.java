package com.alick.ffmpeglearn2.utils;

import android.content.Context;
import android.util.TypedValue;

/**
 * 尺寸转换
 * @email 937211946@qq.com
 * @author xjq
 * @date 2015年12月3日 下午6:03:28
 */
public class DensityUtil {
	/**
	 * 将px值转换为dip或dp值，保证尺寸大小不变
	 *
	 */
	public static int px2dp(Context context, float pxVal) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxVal / scale);
	}

	/**
	 * 将dip或dp值转换为px值，保证尺寸大小不变
	 *
	 */
	public static int dp2px(Context context, float dpVal) {
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
				dpVal, context.getResources().getDisplayMetrics());
	}

	/**
	 * 将px值转换为sp值，保证文字大小不变
	 *
	 */
	public static int px2sp(Context context, float pxVal) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxVal / scale);
	}

	/**
	 * 将sp值转换为px值，保证文字大小不变
	 *
	 * @param spVal
	 */
	public static int sp2px(Context context, float spVal) {
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP,
				spVal, context.getResources().getDisplayMetrics());
	}


}
