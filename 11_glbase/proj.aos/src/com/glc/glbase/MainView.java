package com.glc.glbase;

import org.egl14.*;
import android.opengl.*;
//import javax.microedition.khronos.egl.*;
//import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
//import android.opengl.GLSurfaceView;
import android.util.Log;


class MainView extends GLSurfaceViewEGL14 {
	static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	static final int EGL_OPENGL_ES2_BIT = 4;
	static final int EGL_RECORDABLE_ANDROID = 0x00003142;		// API 26

	private static String TAG = "MAINVIEW";
	public EGLSurface input_srf = null;
	private EGLSurface m_cur_srf = null;

	private static boolean checkEglError(String msg) {
		int error;
		boolean ret = false;
		while ((error = EGL14.eglGetError()) != EGL14.EGL_SUCCESS) {
			Log.e(TAG, String.format("%s: EGL error: 0x%x", msg, error));
			ret = true;
		}
		return ret;
 	}

	public MainView(Context context) {
		super(context);
		setEGLContextFactory(new ContextFactory());
		//setEGLConfigChooser( new ConfigChooser() );
		setRenderer(new Renderer());
	}

	private static class ContextFactory implements GLSurfaceViewEGL14.EGLContextFactory 	{
		public EGLContext createContext(EGLDisplay display, EGLConfig eglConfig) {
			Log.w(TAG, "creating OpenGL ES 2.0 context");
			if(checkEglError("Before eglCreateContext"))
				return null;

			int[] attrib_list = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL14.EGL_NONE };
			EGLContext context = EGL14.eglCreateContext(display, eglConfig, EGL14.EGL_NO_CONTEXT, attrib_list, 0);
			if(checkEglError("After eglCreateContext"))
				return null;

			return context;
		}

		public void destroyContext(EGLDisplay display, EGLContext context) {
			EGL14.eglDestroyContext(display, context);
			checkEglError("eglDestroyContext");
		}
	}

	private static class Renderer implements IRendererEGL14 {
		public void onSurfaceCreated() {
			MainActivity.main_act.appInit();
		}
		public void onSurfaceChanged(final int width, final int height) {
			MainActivity.main_act.appDisplaySize(width, height);
		}
		public void onDrawFrame() {
			MainActivity.main_act.appDraw();
		}
		public void onDestroy() {
			MainActivity.main_act.appDestroy();
		}
	}

	/*
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
	*/

	private static class SurfaceFactory implements GLSurfaceViewEGL14.EGLWindowSurfaceFactory {
		public EGLSurface createWindowSurface(EGLDisplay display, EGLConfig config, Object nativeWindow) {
			int[] surfaceAttribs = { EGL14.EGL_NONE };
			EGLSurface eglSurface = EGL14.eglCreateWindowSurface(display, config, nativeWindow, surfaceAttribs, 0);
			checkEglError("eglCreateWindowSurface");
			return eglSurface;
		}
		public void destroySurface(EGLDisplay display, EGLSurface surface) {
			EGL14.eglDestroySurface(display, surface);
			checkEglError("eglDestroySurface");
		}
	}
}
