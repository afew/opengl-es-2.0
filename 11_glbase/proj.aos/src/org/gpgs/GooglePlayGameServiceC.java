package org.gpgs;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class GooglePlayGameServiceC
{
    public static native void nativeInitGPGS(Activity act);
    public static native void nativeOnActivityResult(Activity activity, int requestCode, int resultCode, Intent data);
    public static native void nativeOnActivityCreated(Activity activity, Bundle savedInstanceState);
    public static native void nativeOnActivityDestroyed(Activity activity);
    public static native void nativeOnActivityPaused(Activity activity);
    public static native void nativeOnActivityResumed(Activity activity);
    public static native void nativeOnActivitySaveInstanceState(Activity activity, Bundle outState);
    public static native void nativeOnActivityStarted(Activity activity);
    public static native void nativeOnActivityStopped(Activity activity);

    // Implemented in C++.
	public static void InitGPGS(Activity act)
    {
    }
    public static void OnActivityResult(Activity activity, int requestCode, int resultCode, Intent data)
    {
    }
    public static void OnActivityCreated(Activity activity, Bundle savedInstanceState)
    {
    }
    public static void OnActivityDestroyed(Activity activity)
    {
    }
    public static void OnActivityPaused(Activity activity)
    {
    }
    public static void OnActivityResumed(Activity activity)
    {
    }
    public static void OnActivitySaveInstanceState(Activity activity, Bundle outState)
    {
    }
    public static void OnActivityStarted(Activity activity)
    {
    }
    public static void OnActivityStopped(Activity activity)
    {
    }
}
