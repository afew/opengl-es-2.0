package com.glc.media;

import com.glc.glbase.*;
import com.glc.glbase.MainActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.*;
import com.google.android.gms.games.video.Videos;

import org.egl14.GLSurfaceViewEGL14;


public class SimpleMediaRecording
{
	private static final String  TAG = SimpleMediaRecording.class.toString();
	private static final int   GPGS_RECORDING_REQ     =  20000;
	private static final int   GPGS_OK                =  0;
	private static final int   GPGS_RECORDING_START   =  1;
	private static final int   GPGS_RECORDING_STOP    =  2;
	private static final int   GPGS_RECORDING_CANCEL  =  3;
	public static final int    GPGS_RECORDING_COMPLETE=  4;

	private static final int   GPGS_ERR               = -1;
	private static final int   GPGS_ERR_CONNECTED     = -2;			// client did not connect
	private static final int   GPGS_ERR_APILEVEL      = -3;			// not support api level
	private static final int   GPGS_ERR_DEVICE        = -4;			// not support device	

	//----------------------------------------------------------------------------------------------

	private static GoogleApiClient gpgs_googleClient  = null;
	private static long            gpgs_addr_sender  = 0;		// callback object pointer
	public static native void nativeCallback(final long addr_sender, final int message);

	public  void nativeCallback(final int message)
	{
		final long sender = gpgs_addr_sender;
		GLSurfaceViewEGL14 view = MainActivity.getMainView();
		view.queueEvent(new Runnable()
		{
			@Override public void run()
			{
				nativeCallback(sender, message);
			}
		});
	}


	//----------------------------------------------------------------------------------------------

	// java call
	public static void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(GPGS_RECORDING_REQ != requestCode)
			return;

		if(-1 != resultCode)
		{
			nativeCallback(gpgs_addr_sender, GPGS_RECORDING_CANCEL);
			return;
		}

		AppUtil.Toast("start VideoRecording --------------");
		nativeCallback(gpgs_addr_sender, GPGS_RECORDING_START);
	}

	// java or c++ calling
	public int startRecording(long addr_sender)
	{
		if(!isConnected())
			return GPGS_ERR_CONNECTED;
		if(!hasLollipopApi())
			return GPGS_ERR_APILEVEL;

		//if( GamesStatusCodes.STATUS_OK != Games.Videos.isCaptureAvailable(gpgs_googleClient, com.google.android.gms.games.video.VideoConfiguration.CAPTURE_MODE_FILE))
		//	return GPGS_ERR_DEVICE;

		gpgs_googleClient  = MainActivity.getGoolgeClient();


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
						nativeCallback(GPGS_RECORDING_STOP);
						nativeCallback(GPGS_RECORDING_COMPLETE);
						break;
					default:
				}
			}
		});
		Intent intent = Games.Videos.getCaptureOverlayIntent(gpgs_googleClient);
		MainActivity.getMainActivity().startActivityForResult(intent, GPGS_RECORDING_REQ);
		return GPGS_OK;
	}


	//----------------------------------------------------------------------------------------------

	private static boolean isConnected()
	{
		return (gpgs_googleClient != null && gpgs_googleClient.isConnected());
	}
	public static boolean hasLollipopApi()
	{
		return android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP;
	}
}

