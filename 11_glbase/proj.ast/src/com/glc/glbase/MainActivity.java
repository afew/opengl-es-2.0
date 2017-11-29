package com.glc.glbase;

import java.io.File;
import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
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

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.games.Player;
import com.google.android.gms.games.PlayersClient;
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

	public static final String  TAG = "JPP";
	public static MainActivity  main_act = null;
	public static AssetManager  main_ast = null;
	public static MainView      main_view= null;

	public static int	dsp_w = 0;
	public static int	dsp_h = 0;

	// Client used to sign in with Google APIs
	private GoogleSignInClient mGoogleSignInClient;

	// Client variables
	private AchievementsClient mAchievementsClient;
	private LeaderboardsClient mLeaderboardsClient;
	private PlayersClient mPlayersClient;

	// request codes we use when invoking an external activity
	private static final int RC_UNUSED = 5001;
	private static final int RC_SIGN_IN = 9001;


	private boolean isSignedIn() {
		return GoogleSignIn.getLastSignedInAccount(this) != null;
	}

	private void onConnected(GoogleSignInAccount googleSignInAccount) {
		Log.d(TAG, "onConnected(): connected to Google APIs");


		// Set the greeting appropriately on main menu
		mPlayersClient.getCurrentPlayer()
		.addOnCompleteListener(new OnCompleteListener<Player>() {
			@Override
			public void onComplete(Task<Player> task) {

				if (task.isSuccessful())
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

	}

	private void onDisconnected() {
		Log.d(TAG, "onDisconnected()");
	}

	private void signInSilently() {
		Log.d(TAG, "signInSilently()");

		mGoogleSignInClient.silentSignIn().addOnCompleteListener(this,
		new OnCompleteListener<GoogleSignInAccount>()
		{
			@Override
			public void onComplete(Task<GoogleSignInAccount> task)
			{
				if (task.isSuccessful())
				{
					Log.d(TAG, "signInSilently(): success");
					onConnected(task.getResult());
				}
				else
				{
					Log.d(TAG, "signInSilently(): failure", task.getException());
					onDisconnected();
				}
			}
		});
	}
	@Override protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		main_act = this;
		main_ast = this.getAssets();

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


		// Create the client used to sign in to Google services.
		mGoogleSignInClient = GoogleSignIn.getClient(this, new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN).build());
		startActivityForResult(mGoogleSignInClient.getSignInIntent(), RC_SIGN_IN);

	}
	@Override protected void onPause()
	{
		super.onPause();
		main_view.onPause();
	}

	@Override protected void onResume()
	{
		super.onResume();
		main_view.onResume();

		signInSilently();
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


	//------------------------------------------------------------------------------------------------------------------

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

	@Override public boolean dispatchTouchEvent( MotionEvent event )
	{
		float x = event.getX();
		float y = event.getY();

		int keyAction = event.getAction();
		int index     = event.getActionIndex();
		switch(keyAction & MotionEvent.ACTION_MASK)
		{
			case MotionEvent.ACTION_DOWN:
			{
				//Log.i(TAG, "MotionEvent.ACTION_DOWN:: index: " + index + ", action: " + MotionEvent.ACTION_UP);
				touchEvent(index, 1, x, y);
			}break;

			case MotionEvent.ACTION_UP:
			{
				//Log.i(TAG, "MotionEvent.ACTION_UP:: index: " + index + ", action: " + MotionEvent.ACTION_UP);
				touchEvent(index, 2, x, y);
			}break;
		}
		boolean result =  super.dispatchTouchEvent(event);
		return result;
	}
}
