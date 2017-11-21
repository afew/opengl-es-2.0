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
import android.util.DisplayMetrics;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity
{
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
	}

	public void appInit()
	{
		MainActivity.init((Object)MainActivity.main_ast, MainActivity.dsp_w, MainActivity.dsp_h);
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
}
