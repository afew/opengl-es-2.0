package com.glc.glbase;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;


class MainView extends GLSurfaceView {
	private static String TAG = MainView.class.toString();
	static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	static final int EGL_OPENGL_ES2_BIT = 4;
	static final int EGL_RECORDABLE_ANDROID = 0x00003142;		// API 26

	private static boolean checkEglError(EGL10 egl, String msg)
	{
		int error;
		boolean ret = false;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
		{
			Log.e(TAG, String.format("%s: EGL error: 0x%x", msg, error));
			ret = true;
		}
		return ret;
	}

	public MainView(Context context)
	{
		super(context);
		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser(new ConfigChooser());
		setPreserveEGLContextOnPause(true);
		setRenderer(new Renderer());
	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			Log.w(TAG, "creating OpenGL ES 2.0 context");
			if(checkEglError(egl, "Before eglCreateContext"))
				return null;

			int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			if(checkEglError(egl, "After eglCreateContext"))
				return null;

			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
			egl.eglDestroyContext(display, context);
			checkEglError(egl, "eglDestroyContext");
		}
	}

	private static class Renderer implements GLSurfaceView.Renderer {
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			MainActivity.main_act.appInit();
		}
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			MainActivity.main_act.appDisplaySize(width, height);
		}
		public void onDrawFrame(GL10 gl) {
			//GLES20.glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
			//GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT|GLES20.GL_DEPTH_BUFFER_BIT);
			MainActivity.main_act.appDraw();
		}
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
		public ConfigChooser() {
			mRedSize   = 8;
			mGreenSize = 8;
			mBlueSize  = 8;
			mAlphaSize = 8;
			mDepthSize = 16;
			mStencilSize = 0;
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
			int[] configAttrib = {
					EGL10.EGL_RED_SIZE    , mRedSize,
					EGL10.EGL_GREEN_SIZE  , mGreenSize,
					EGL10.EGL_BLUE_SIZE   , mBlueSize,
					EGL10.EGL_ALPHA_SIZE  , mAlphaSize,
					EGL10.EGL_DEPTH_SIZE  , mDepthSize,
					EGL10.EGL_STENCIL_SIZE, mStencilSize,
					EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
					EGL10.EGL_NONE
				};
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, configAttrib, null, 0, num_config);
			int numConfigs = num_config[0];
			if (numConfigs <= 0)
				throw new IllegalArgumentException("No configs match configSpec");
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, configAttrib, configs, numConfigs, num_config);
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
			for(EGLConfig config : configs) {
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);
				if (d < mDepthSize || s < mStencilSize)
					continue;
				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE  , 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE , 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);
				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
					return config;
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) {
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
				return mValue[0];
			return defaultValue;
		}
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}
}
