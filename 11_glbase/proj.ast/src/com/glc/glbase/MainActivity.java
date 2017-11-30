package com.glc.glbase;

import org.gpgs.*;

import java.io.File;
import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.Intent;
import android.graphics.Point;
import android.graphics.Rect;
import android.opengl.GLES20;
import android.os.Bundle;
import android.util.*;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.Toast;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.common.api.GoogleApiActivity;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayersClient;
import com.google.android.gms.games.VideosClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

public class MainActivity extends Activity
{
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

	public static int	dsp_w = 0;
	public static int	dsp_h = 0;


	// Client used to sign in with Google APIs
	private GoogleSignInClient gpgs_googleClient;
	private AchievementsClient gpgs_achievements;
	private LeaderboardsClient gpgs_leaderboard;
	private PlayersClient      gpgs_players;

	public static final int RC_UNUSED   = 5001;
	public static final int RQS_SIGN_IN = 9001;
	public static final int RQS_VIDEO   = 9011;


	@Override protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		main_act = this;
		main_ast = this.getAssets();
		Util.init(this);
		GooglePlayGameServiceC.InitGPGS(this);
		GooglePlayGameServiceC.OnActivityCreated(this, savedInstanceState);


		// setup the display size
		android.util.DisplayMetrics mtr = new DisplayMetrics();
		android.view.WindowManager  wnm = this.getWindowManager();
		android.view.Display		dsp = wnm.getDefaultDisplay();
		dsp.getMetrics(mtr);

		Point dspPoint = new Point();
		dsp.getSize(dspPoint);
		MainActivity.dsp_w = dspPoint.x;
		MainActivity.dsp_h = dspPoint.y;

		System.loadLibrary("g-pack");
		MainActivity.main_view = new MainView(getApplication());
		setContentView(MainActivity.main_view);
	}

	@Override protected void onDestroy()
	{
		super.onDestroy();
		GooglePlayGameServiceC.OnActivityDestroyed(this);
	}
	@Override protected void onPause()
	{
		super.onPause();
		main_view.onPause();
		GooglePlayGameServiceC.OnActivityPaused(this);
	}
	@Override protected void onResume()
	{
		super.onResume();
		main_view.onResume();
		silentSignIn();

		GooglePlayGameServiceC.OnActivityResumed(this);
	}
	@Override protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		GooglePlayGameServiceC.OnActivitySaveInstanceState(this, outState);
	}
	@Override protected void onStart()
	{
		super.onStart();
		GooglePlayGameServiceC.OnActivityStarted(this);
	}
	@Override protected void onStop()
	{
		super.onStop();
		GooglePlayGameServiceC.OnActivityStopped(this);
	}

	public void appInit()
	{
		MainActivity.init((Object)MainActivity.main_ast, MainActivity.dsp_w, MainActivity.dsp_h);
	}
	public void appDestroy()
	{
		Util.LOGD("appDestroy: ");
	}
	public void appDisplaySize(int width, int height)
	{
		Util.LOGD("with: " + width + "  height: " + height);
	}

	public void appDraw()
	{
		update();
		draw();
	}

	@Override public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		int keyAction = event.getAction();
		int index     = event.getKeyCode();
		switch (keyCode)
		{
			case KeyEvent.KEYCODE_BACK:
			{
				keyEvent(index, MotionEvent.ACTION_DOWN);
				return true;
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
					startSignIn();
				else
					startRecording();
				break;
			}
		}
		boolean result =  super.dispatchTouchEvent(event);
		return result;
	}

	@Override  public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(requestCode == RQS_SIGN_IN)
		{
			signInResult(requestCode, resultCode, data);
			return;
		}
		super.onActivityResult(requestCode, resultCode, data);
	}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

	public void startSignIn()
	{
		if(isSignedIn())
		{
			onConnected();
			return;
		}

		// Configure sign-in to request the user's ID, email address, and basic  profile. ID and basic profile are included in DEFAULT_SIGN_IN.
		GoogleSignInOptions gso =
		new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
		.requestEmail()
		.build();

		if(null == gso)
		{
			Log.w(TAG, "startSignIn()::GoogleSignInOptions failed");
			return;
		}

		gpgs_googleClient = GoogleSignIn.getClient(this, gso);
		if(null == gpgs_googleClient)
		{
			Log.w(TAG, "startSignIn()::GoogleSignIn.getClient failed");
			return;
		}

		GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(this);

		// Create the client used to sign in to Google services.
		startActivityForResult(gpgs_googleClient.getSignInIntent(), RQS_SIGN_IN);
		Log.i(TAG, "start signInIntent() -------------------------------------");
	}
	private void silentSignIn()
	{
		gpgs_googleClient = GoogleSignIn.getClient(this, GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN);
		if(null == gpgs_googleClient)
		{
			Log.w(TAG, "silentSignIn()::GoogleSignIn.getClient failed");
			return;
		}

		gpgs_googleClient.silentSignIn().addOnCompleteListener(this, new OnCompleteListener<GoogleSignInAccount>()
		{
			@Override public void onComplete(Task<GoogleSignInAccount> task)
			{
				if(task.isSuccessful())
				{
					GoogleSignInAccount account = task.getResult();
					Log.i(TAG, "silentSignIn::onComplete(): task successs: " + account.toString());
				}
				else
				{
					Exception e = task.getException();
					Log.e(TAG, "silentSignIn::onComplete(): task error: " + e.toString());
				}
			}
		});
		Log.d(TAG, "silentSignIn() --------------------------------------------------------------");
	}

	private void signInResult(int requestCode, int resultCode, Intent data)
	{
		if(RQS_SIGN_IN != requestCode)
			return;

		Task<GoogleSignInAccount> completedTask = GoogleSignIn.getSignedInAccountFromIntent(data);
		try
		{
			GoogleSignInAccount account = completedTask.getResult(ApiException.class);

			Log.i(TAG, "################################################################");
			Log.i(TAG, "");
			Log.i(TAG, "signed in success: " + account.toString());
			Util.Toast("signed in success: " + account.toString());
			Log.i(TAG, "");
		}
		catch (ApiException e)
		{
			//DEVELOPER_ERROR
			Log.w(TAG, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");
			Log.w(TAG, "");
			Log.w(TAG, "signInResult:failed code=" + e.getStatusCode());
			Util.Toast("signInResult:failed code=" + e.getStatusCode());
			Log.w(TAG, "");
		}
	}

	private boolean isSignedIn()
	{
		return GoogleSignIn.getLastSignedInAccount(this) != null;
	}

	private void onConnected()
	{
		if(!isSignedIn())
			return;

		GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(this);
		Log.d(TAG, "onConnected(): account: " + account.toString());

		/*
		gpgs_players = PlayersClient.getPlayersClient(this, account);
		gpgs_players.getCurrentPlayer()
		.addOnCompleteListener(new OnCompleteListener<Player>()
		{
			@Override public void onComplete(Task<Player> task)
			{
				if(task.isSuccessful())
				{
					Log.i(TAG, "onComplete(): task successs");
				}
				else
				{
					Exception e = task.getException();
					Log.e(TAG, "onComplete(): task error");
				}

			}
		});
		*/
	}

	private void onDisconnected()
	{
		Log.d(TAG, "onDisconnected()");
	}

	public void startRecording()
	{
		int result = com.google.android.gms.common.GooglePlayServicesUtil.isGooglePlayServicesAvailable(this);
		if(com.google.android.gms.common.ConnectionResult.SUCCESS != result)
			return;

		if(!isSignedIn())
			return;

		GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(this);
		Log.i(TAG, "start recording () 0 : " +account.toString());
		VideosClient videoClient = Games.getVideosClient(this, account);
		Log.i(TAG, "start recording () 1");
	}
}
