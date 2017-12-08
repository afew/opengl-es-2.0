package com.glc.glbase;

import android.app.Activity;
import android.content.res.AssetManager;
import android.content.Intent;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class MainActivity extends Activity
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

	public static GLSurfaceView getMainSurfaceView()
	{
		return main_view;
	}
	public static Activity getMainActivity()
	{
		return main_act;
	}

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

	@Override protected void onStart()
	{
		super.onStart();
	}

	@Override protected void onStop()
	{
		super.onStop();
	}
	@Override protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
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
