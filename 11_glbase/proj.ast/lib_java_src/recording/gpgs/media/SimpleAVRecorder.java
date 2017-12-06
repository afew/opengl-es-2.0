
package gpgs.media;

import android.app.Activity;
import android.content.Intent;
import android.preference.PreferenceManager.OnActivityResultListener;
import android.opengl.GLSurfaceView;
import android.util.Log;

//import org.cocos2dx.lib.Cocos2dxActivity;
//import org.cocos2dx.lib.Cocos2dxHelper;
import com.glc.glbase.MainActivity;

import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.video.Videos;

import pwork.service.GoogleSignIn;

//public class SimpleAVRecorder implements OnActivityResultListener
public class SimpleAVRecorder
{
	public static final String  TAG = SimpleAVRecorder.class.toString();
	public static final int AVRECORDER_EVENT_OK       = 0x0000;
	public static final int AVRECORDER_EVENT_NONE     = 0x0000;
	public static final int AVRECORDER_EVENT_START    = 0x0001;
	public static final int AVRECORDER_EVENT_STOP     = 0x0002;
	public static final int AVRECORDER_EVENT_CANCEL   = 0x0003;
	public static final int AVRECORDER_EVENT_COMPLETE = 0x0004;
	public static final int AVRECORDER_EVENT_ETC      = 0xFFFF;
	public static final int AVRECORDER_ERR_CONNECT    = -2;			// client did not connect
	public static final int AVRECORDER_ERR_APILEVEL   = -3;			// not support api level
	public static final int AVRECORDER_ERR_DEVICE     = -4;			// not support device

	// for google play service
	public static Activity         main_act          = null;
	public static GLSurfaceView    main_surfaceView  = null;
	private static GoogleApiClient gpgs_googleClient = null;
	public static final int        GPGS_RECORDING  = 20000;

	// native callback
	public static native void nativeCallback(String addr_sender, int message);
	private static String cb_addr_sender  = "0";
	public static SimpleAVRecorder mAvrecorder = null;

	public static void nativeCallback(final int message) {
		final String sender = cb_addr_sender;
		main_surfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				nativeCallback(sender, message);
			}
		});
	}

	public static int startRecording(String addr_sender)
	{
		cb_addr_sender  = addr_sender;
		if(null == mAvrecorder)
		{
			mAvrecorder = new SimpleAVRecorder();
			//Cocos2dxHelper.addOnActivityResultListener(mAvrecorder);
		}
		main_act = MainActivity.getMainActivity();
		main_surfaceView = MainActivity.getMainSurfaceView();

		// for cocos2dx
		//Cocos2dxActivity activity = (Cocos2dxActivity)Cocos2dxActivity.getContext();
		//main_act = (Activity)activity;
		//main_surfaceView = (GLSurfaceView)activity.getGLSurfaceView();

		gpgs_googleClient  = GoogleSignIn.getGoolgeClient();

		if(!isSignedIn())
			return AVRECORDER_ERR_CONNECT;
		//Log.i(TAG, "start VideoRecording");
		if(!hasLollipopApi())
			return AVRECORDER_ERR_APILEVEL;

		Games.Videos.registerCaptureOverlayStateChangedListener(gpgs_googleClient, new Videos.CaptureOverlayStateListener()
		{
			public void onCaptureOverlayStateChanged (int overlayState)
			{
				Log.i(TAG, "registerCaptureOverlayStateChangedListener: --------------------------------------------------------");
				Log.i(TAG, "overlayState: " + overlayState);
				switch (overlayState)
				{
					case Videos.CAPTURE_OVERLAY_STATE_SHOWN:
						Log.i(TAG, "CAPTURE_OVERLAY_STATE_SHOWN");
						break;
					case Videos.CAPTURE_OVERLAY_STATE_CAPTURE_STARTED:
						Log.i(TAG, "CAPTURE_OVERLAY_STATE_CAPTURE_STARTED");
						break;
					case Videos.CAPTURE_OVERLAY_STATE_CAPTURE_STOPPED:
						Log.i(TAG, "CAPTURE_OVERLAY_STATE_CAPTURE_STOPPED");
					case Videos.CAPTURE_OVERLAY_STATE_DISMISSED:
						Log.i(TAG, "CAPTURE_OVERLAY_STATE_DISMISSED");
						nativeCallback(AVRECORDER_EVENT_STOP);
						nativeCallback(AVRECORDER_EVENT_COMPLETE);
						break;
					default:
				}
			}
		});
		Intent intent = Games.Videos.getCaptureOverlayIntent(gpgs_googleClient);
		main_act.startActivityForResult(intent, GPGS_RECORDING);
		return AVRECORDER_EVENT_OK;
	}
	//@Override public boolean onActivityResult(int requestCode, int resultCode, Intent data)
	public static boolean onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(requestCode != GPGS_RECORDING)
			return false;

		if(0 == resultCode)	//cancel
		{
			Log.i(TAG, "AVRecording canceled ----------------------------------------------------");
			nativeCallback(AVRECORDER_EVENT_CANCEL);
			return true;
		}
		Log.i(TAG, "start AVRecording -------------------   ---------------------------------");
		nativeCallback(AVRECORDER_EVENT_START);
		return true;
	}

	private static boolean isSignedIn()
	{
		if(null == main_act)
			return false;
		if(null == gpgs_googleClient)
			return false;
		return gpgs_googleClient.isConnected();
	}
	private static boolean hasLollipopApi()
	{
		return android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP;
	}
}
