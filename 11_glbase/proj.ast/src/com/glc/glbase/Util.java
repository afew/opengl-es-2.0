package com.glc.glbase;

import android.util.Log;

public class Util
{
	public static final String TAG = "JPP";

	public static void LOGD(final String m)                 { Log.d(TAG, "========:::: " + m); }
	public static void LOGD(final String t, final String m) { Log.d(  t, "========:::: " + m); }
	public static void LOGI(final String m)                 { Log.i(TAG, "++++++++:::: " + m); }
	public static void LOGI(final String t, final String m) { Log.i(  t, "++++++++:::: " + m); }
	public static void LOGW(final String m)                 { Log.w(TAG, "$$$$$$$$:::: " + m); }
	public static void LOGW(final String t, final String m) { Log.w(  t, "$$$$$$$$:::: " + m); }
	public static void LOGE(final String m)                 { Log.e(TAG, "@@@@@@@@:::: " + m); }
	public static void LOGE(final String t, final String m) { Log.e(  t, "@@@@@@@@:::: " + m); }
}
