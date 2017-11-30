package com.glc.glbase;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;

public class Util
{
	public static final String TAG = "JPP";
	public static Activity main_act = null;
	public static void init(Activity act) { main_act = act; }

	public static void LOGD(final String m)                 { Log.d(TAG, "========:::: " + m); }
	public static void LOGD(final String t, final String m) { Log.d(  t, "========:::: " + m); }
	public static void LOGI(final String m)                 { Log.i(TAG, "++++++++:::: " + m); }
	public static void LOGI(final String t, final String m) { Log.i(  t, "++++++++:::: " + m); }
	public static void LOGW(final String m)                 { Log.w(TAG, "$$$$$$$$:::: " + m); }
	public static void LOGW(final String t, final String m) { Log.w(  t, "$$$$$$$$:::: " + m); }
	public static void LOGE(final String m)                 { Log.e(TAG, "@@@@@@@@:::: " + m); }
	public static void LOGE(final String t, final String m) { Log.e(  t, "@@@@@@@@:::: " + m); }

	synchronized public static void Toast(final String message)
	{
		Util.main_act.runOnUiThread(new Runnable()
		{
			@Override public void run()
			{
				Toast.makeText(Util.main_act, message, Toast.LENGTH_SHORT).show();
			}
		});
	}
}
