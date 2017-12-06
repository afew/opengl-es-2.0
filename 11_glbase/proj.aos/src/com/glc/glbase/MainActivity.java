package com.glc.glbase;

import android.app.Activity;
import android.content.res.AssetManager;
import android.content.Intent;
import android.graphics.Point;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.Player;


public class MainActivity
	extends Activity
	implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener
{
	// for native
	public static native void keyEvent(int index, int action);
	public static native void touchEvent(int index, int action, float x, float y);
	public static native void init(Object assetManager, int width, int height);
	public static native void destroy();
	public static native void update();
	public static native void draw();

	public static final String  TAG = MainActivity.class.toString();
	public static MainActivity  main_act = null;
	public static AssetManager  main_ast = null;
	public static MainView      main_view= null;
	public static int           main_screen_w = 0;
	public static int           main_screen_h = 0;

	// for google play service
	private GoogleApiClient    gpgs_googleClient = null;
	private boolean            gpgs_resolving = false;		// for resolving connection

	private static final int   GPGS_RESOLUTION = 10000;
	private static final int   GPGS_RECORDING  = 20000;


	@Override protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		main_act = this;
		main_ast = this.getAssets();
		AppUtil.init(this);

		// setup the display size
		android.util.DisplayMetrics mtr = new DisplayMetrics();
		android.view.WindowManager  wnm = this.getWindowManager();
		android.view.Display		dsp = wnm.getDefaultDisplay();
		dsp.getMetrics(mtr);

		Point dspPoint = new Point();
		dsp.getSize(dspPoint);
		MainActivity.main_screen_w = dspPoint.x;
		MainActivity.main_screen_h = dspPoint.y;

		System.loadLibrary("g-pack");
		MainActivity.main_view = new MainView(getApplication());
		setContentView(main_view);

		// Create the Google API Client with access to Games
		gpgs_googleClient = new GoogleApiClient.Builder(this)
				.addConnectionCallbacks(this)
				.addOnConnectionFailedListener(this)
				.addApi(Games.API)
				.addScope(Games.SCOPE_GAMES)
				//.addApi(Plus.API)
				//.addScope(Plus.SCOPE_PLUS_LOGIN)
				//.addScope(Plus.SCOPE_PLUS_PROFILE)
				.build();
	}

	@Override protected void onDestroy()
	{
		super.onDestroy();
	}
	@Override protected void onResume()
	{
		super.onResume();
		main_view.onResume();
	}
	@Override protected void onPause()
	{
		super.onPause();
		main_view.onPause();
	}

	//----------------------------------------------------------------------------------------------

	@Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		int keyAction = event.getAction();
		int index     = event.getKeyCode();
		switch (keyCode)
		{
			case KeyEvent.KEYCODE_BACK:
			{
				keyEvent(index, MotionEvent.ACTION_DOWN);
				break;
			}
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override public boolean dispatchTouchEvent( MotionEvent event)
	{
		float x = event.getX();
		float y = event.getY();

		int keyAction = event.getAction();
		int index     = event.getActionIndex();
		switch(keyAction & MotionEvent.ACTION_MASK)
		{
			case MotionEvent.ACTION_DOWN:
			{
				touchEvent(index, 1, x, y);
				break;
			}
			case MotionEvent.ACTION_UP:
			{
				touchEvent(index, 2, x, y);
				if(!isSignedIn())
				{
					signIn();
				}
				else
				{
					startVideoRecording();
				}
				break;
			}
			default:
				break;
		}
		boolean result =  super.dispatchTouchEvent(event);
		return result;
	}

	//----------------------------------------------------------------------------------------------

	// siginning and recording

	private boolean isSignedIn()
	{
		return (gpgs_googleClient != null && gpgs_googleClient.isConnected());
	}

	@Override protected void onStart()
	{
		super.onStart();
		AppUtil.LOGI("onStart(): connecting");
		gpgs_googleClient.connect();
	}

	@Override protected void onStop()
	{
		super.onStop();
		AppUtil.LOGI("onStop(): disconnecting");
		signOut();
	}
	@Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(requestCode == GPGS_RESOLUTION)
		{
			if (resultCode == Activity.RESULT_OK)
			{
				gpgs_googleClient.connect();
			}
		}
		else if(requestCode == GPGS_RECORDING)
		{
			AppUtil.Toast("start VideoRecording() :: request result: " + resultCode);
		}
		super.onActivityResult(requestCode, resultCode, data);
	}

	public void signIn()
	{
		gpgs_resolving = false;
		gpgs_googleClient.connect();
		AppUtil.LOGI("start signIn -------------------------------------");
	}

	public void signOut()
	{
		Games.signOut(gpgs_googleClient);
		if (gpgs_googleClient.isConnected())
		{
			gpgs_googleClient.disconnect();
		}
		AppUtil.LOGI("sign Out    -------------------------------------");
	}

	@Override public void onConnected(Bundle bundle)
	{
		AppUtil.LOGI("onConnected(): connected to Google APIs");

		Player p = Games.Players.getCurrentPlayer(gpgs_googleClient);
		if(p == null)
		{
			AppUtil.Toast("onConnected(): player : NULL");
		}
		else
		{
			AppUtil.Toast("onConnected(): player : " + p.getDisplayName() );
		}
	}

	@Override public void onConnectionSuspended(int i)
	{
		AppUtil.LOGI("onConnectionSuspended(): attempting to connect");
		gpgs_googleClient.connect();
	}

	@Override public void onConnectionFailed(ConnectionResult result)
	{
		AppUtil.LOGI("onConnectionFailed(): " + result.toString());
		if (gpgs_resolving)
		{
			AppUtil.LOGW("onConnectionFailed(): already resolving");
			return;
		}

		gpgs_resolving = true;			// startResolutionForResult 재시도 방지
		if (result.hasResolution())
		{
			try
			{
				result.startResolutionForResult(this, GPGS_RESOLUTION);
				AppUtil.LOGI("onConnectionFailed(): startResolutionForResult");
			}
			catch (Exception e)
			{
				AppUtil.LOGW("onConnectionFailed(): startResolutionForResult:: failed");
			}
		}
	}

	public void startVideoRecording()
	{
		if(!isSignedIn())
			return;
		AppUtil.LOGI("start VideoRecording");
		if(!hasLollipopApi())
			return;

		Intent intent = Games.Videos.getCaptureOverlayIntent(gpgs_googleClient);
		startActivityForResult(intent, GPGS_RECORDING);
	}
	public static boolean hasLollipopApi()
	{
		return android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP;
	}

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------

	public void appInit()
	{
		MainActivity.init((Object)MainActivity.main_ast, MainActivity.main_screen_w, MainActivity.main_screen_h);
	}
	public void appDestroy()
	{
		AppUtil.LOGD("appDestroy: ");
	}
	public void appDisplaySize(int width, int height)
	{
		AppUtil.LOGD("with: " + width + "  height: " + height);
	}
	public void appDraw()
	{
		update();
		draw();
	}

}

