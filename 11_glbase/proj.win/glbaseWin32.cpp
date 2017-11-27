/*!*********************************************************************************************************************
\File         OGLESHelloAPI_Windows.cpp
\Title        OpenGL ES ReadPixel Tutorial
\Author       PowerVR by Imagination, Developer Technology Team
\Copyright    Copyright (c) Imagination Technologies Limited.
\brief		  Basic Tutorial that shows step-by-step how to initialize OpenGL ES 2.0, use it for drawing a triangle and terminate it.
              Entry Point: WinMain
***********************************************************************************************************************/

#if defined(_DEBUG)
  #pragma comment(lib, "lgpwn_math30_.lib")
#else
  #pragma comment(lib, "lgpwn_math30.lib")
#endif


#include <windows.h>
#include <stdio.h>

#define GL_GLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <vector>

extern int app_init();
extern int app_destroy();
extern int app_update();
extern int app_draw();


// Windows class name to register
#define	WINDOW_CLASS_NAME "PVRShellClass"

// Name of the application
#define APPLICATION_NAME "opengl es 2.0 base"

// Width and height of the window
const unsigned int WindowWidth  = 1280;
const unsigned int WindowHeight = 768;

HWND g_nativeWindow = NULL;
void* NativeWindow() { return g_nativeWindow; }

const char* AppResourcePath(const char* fileName)
{
	return fileName;
}

/*!*********************************************************************************************************************
\param			nativeWindow                Handle to the window
\param			message                     The message to handle
\param			windowParameters            Additional message information
\param			longWindowParameters        Additional message information
\return	Result code to send to the OS
\brief	Processes event messages for the main window
***********************************************************************************************************************/
LRESULT CALLBACK handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				{
					return 0;
				}
			}
			break;
		}

		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
					SendMessage(hWnd, WM_CLOSE, 0,0);
					return 0;
			}
			break;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 1;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*!*********************************************************************************************************************
\param			nativeWindow                Handle to the window
\param			functionLastCalled          Function which triggered the error
\return	True if no EGL error was detected
\brief	Tests for an EGL error and prints it in a message box.
***********************************************************************************************************************/
bool testEGLError( const char* functionLastCalled)
{
	/*	eglGetError returns the last error that occurred using EGL, not necessarily the status of the last called function. The user has to
		check after every single EGL call or at least once every frame. Usually this would be for debugging only, but for this example
		it is enabled always.
	*/
	EGLint lastError = eglGetError();
	if (lastError != EGL_SUCCESS)
	{
		char stringBuffer[256];
		sprintf(stringBuffer, "%s failed (%x).\n", functionLastCalled, lastError);
		MessageBox(g_nativeWindow, stringBuffer, "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

/*!*********************************************************************************************************************
\param		applicationInstance		    Specific instance of the application
\param[out]	nativeWindow			    Native window type to create
\param[out]	deviceContext			    Native device context to create
\return	Whether the function succeeded or not.
\brief	Creates a native window and display for the application to render into.
***********************************************************************************************************************/
bool createWindowAndDisplay(HINSTANCE applicationInstance, HWND& nativeWindow, HDC& deviceContext)
{
	// Describe the native window in a window class structure
	WNDCLASS nativeWindowDescription;
	nativeWindowDescription.style = CS_HREDRAW | CS_VREDRAW;
	nativeWindowDescription.lpfnWndProc = handleWindowMessages;
	nativeWindowDescription.cbClsExtra = 0;
	nativeWindowDescription.cbWndExtra = 0;
	nativeWindowDescription.hInstance = applicationInstance;
	nativeWindowDescription.hIcon = 0;
	nativeWindowDescription.hCursor = 0;
	nativeWindowDescription.lpszMenuName = 0;
	nativeWindowDescription.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	nativeWindowDescription.lpszClassName = WINDOW_CLASS_NAME;

	// Register the windows class with the OS.
	ATOM registerClass = RegisterClass(&nativeWindowDescription);
	if (!registerClass)
	{
		MessageBox(0, "Failed to register the window class", "Err", MB_OK | MB_ICONEXCLAMATION);
	}

	// Create a rectangle describing the area of the window
	RECT windowRectangle;
	SetRect(&windowRectangle, 0, 0, WindowWidth, WindowHeight);
	AdjustWindowRectEx(&windowRectangle, WS_CAPTION | WS_SYSMENU, false, 0);

	// Create the window from the available information
	nativeWindow = CreateWindow(WINDOW_CLASS_NAME, APPLICATION_NAME, WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
	                            windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top,
	                            NULL, NULL, applicationInstance, NULL);
	if (!nativeWindow)
	{
		MessageBox(0, "Failed to create the window", "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Get the associated device context from the window
	deviceContext = GetDC(nativeWindow);
	if (!deviceContext)
	{
		MessageBox(nativeWindow, "Failed to create the device context", "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

/*!*********************************************************************************************************************
\param		deviceContext               The device context used by the application
\param[out]	eglDisplay				    EGLDisplay created from deviceContext
\return	Whether the function succeeded or not.
\brief	Creates an EGLDisplay from a native device context, and initializes it.
***********************************************************************************************************************/
bool createEGLDisplay(HDC deviceContext, EGLDisplay& eglDisplay)
{
	//	Get an EGL display.
	//	EGL uses the concept of a "display" which in most environments corresponds to a single physical screen. After creating a native
	//	display for a given windowing system, EGL can use this handle to get a corresponding EGLDisplay handle to it for use in rendering.
	//	Should this fail, EGL is usually able to provide access to a default display.

	eglDisplay = eglGetDisplay(deviceContext);
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
	}

	// If a display still couldn't be obtained, return an error.
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		MessageBox(0, "Failed to get an EGLDisplay", "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	//	Initialize EGL.
	//	EGL has to be initialized with the display obtained in the previous step. All EGL functions other than eglGetDisplay
	//	and eglGetError need an initialized EGLDisplay.
	//	If an application is not interested in the EGL version number it can just pass NULL for the second and third parameters, but they
	//	are queried here for illustration purposes.

	EGLint eglMajorVersion, eglMinorVersion;
	if (!eglInitialize(eglDisplay, &eglMajorVersion, &eglMinorVersion))
	{
		MessageBox(0, "Failed to initialize the EGLDisplay", "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

/*!*********************************************************************************************************************
\param		eglDisplay                  The EGLDisplay used by the application
\param[out]	eglConfig                   The EGLConfig chosen by the function
\return	Whether the function succeeded or not.
\brief	Chooses an appropriate EGLConfig and return it.
***********************************************************************************************************************/
bool chooseEGLConfig(EGLDisplay eglDisplay, EGLConfig& eglConfig)
{

	//	Specify the required configuration attributes.
	//	An EGL "configuration" describes the capabilities an application requires and the type of surfaces that can be used for drawing.
	//	Each implementation exposes a number of different configurations, and an application needs to describe to EGL what capabilities it
	//	requires so that an appropriate one can be chosen. The first step in doing this is to create an attribute list, which is an array
	//	of key/value pairs which describe particular capabilities requested. In this application nothing special is required so we can query
	//	the minimum of needing it to render to a window, and being OpenGL ES 2.0 capable.
	const EGLint configurationAttributes[] =
	{
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};


	//	Find a suitable EGLConfig
	//	eglChooseConfig is provided by EGL to provide an easy way to select an appropriate configuration. It takes in the capabilities
	//	specified in the attribute list, and returns a list of available configurations that match or exceed the capabilities requested.
	//	Details of all the possible attributes and how they are selected for by this function are available in the EGL reference pages here:
	//	http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
	//	It is also possible to simply get the entire list of configurations and use a custom algorithm to choose a suitable one, as many
	//	advanced applications choose to do. For this application however, taking the first EGLConfig that the function returns suits
	//	its needs perfectly, so we limit it to returning a single EGLConfig.

	EGLint configsReturned;
	if (!eglChooseConfig(eglDisplay, configurationAttributes, &eglConfig, 1, &configsReturned) || (configsReturned != 1))
	{
		MessageBox(0, "eglChooseConfig() failed.", "Err", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

/*!*********************************************************************************************************************
\param		nativeWindow                A native window that's been created
\param		eglDisplay                  The EGLDisplay used by the application
\param		eglConfig                   An EGLConfig chosen by the application
\param[out]	eglSurface					The EGLSurface created from the native window.
\return	Whether the function succeeds or not.
\brief	Creates an EGLSurface from a native window
***********************************************************************************************************************/
bool createEGLSurface(HWND nativeWindow, EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface& eglSurface)
{

	//	Create an EGLSurface for rendering.
	//	Using a native window created earlier and a suitable eglConfig, a surface is created that can be used to render OpenGL ES calls to.
	//	There are three main surface types in EGL, which can all be used in the same way once created but work slightly differently:
	//	 - Window Surfaces  - These are created from a native window and are drawn to the screen.
	//	 - Pixmap Surfaces  - These are created from a native windowing system as well, but are offscreen and are not displayed to the user.
	//	 - PBuffer Surfaces - These are created directly within EGL, and like Pixmap Surfaces are offscreen and thus not displayed.
	//	The offscreen surfaces are useful for non-rendering contexts and in certain other scenarios, but for most applications the main
	//	surface used will be a window surface as performed below.

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, NULL);
	if (eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	// Check for any EGL Errors
	if (!testEGLError("eglCreateWindowSurface")) {	return false; }
	return true;
}

/*!*********************************************************************************************************************
\param			eglDisplay                  The EGLDisplay used by the application
\param			eglConfig                   An EGLConfig chosen by the application
\param			eglSurface					The EGLSurface created from the native window.
\param[out] 	eglContext                  The EGLContext created by this function
\param			nativeWindow                A native window, used to display error messages
\return	Whether the function succeeds or not.
\brief	Sets up the EGLContext, creating it and then installing it to the current thread.
***********************************************************************************************************************/
bool setupEGLContext(EGLDisplay eglDisplay, EGLConfig eglConfig, EGLSurface eglSurface, EGLContext& eglContext, HWND nativeWindow)
{
	//	Make OpenGL ES the current API.
	//	EGL needs a way to know that any subsequent EGL calls are going to be affecting OpenGL ES,
	//	rather than any other API (such as OpenVG).

	eglBindAPI(EGL_OPENGL_ES_API);
	if (!testEGLError("eglBindAPI")) {	return false; }

	//	Create a context.
	//	EGL has to create what is known as a context for OpenGL ES. The concept of a context is OpenGL ES's way of encapsulating any
	//	resources and state. What appear to be "global" functions in OpenGL actually only operate on the current context. A context
	//	is required for any operations in OpenGL ES.
	//	Similar to an EGLConfig, a context takes in a list of attributes specifying some of its capabilities. However in most cases this
	//	is limited to just requiring the version of the OpenGL ES context required - In this case, OpenGL ES 2.0.
	EGLint contextAttributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	// Create the context with the context attributes supplied
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, contextAttributes);
	if (!testEGLError("eglCreateContext")) {	return false;	}

	//	Bind the context to the current thread.
	//	Due to the way OpenGL uses global functions, contexts need to be made current so that any function call can operate on the correct
	//	context. Specifically, make current will bind the context to the thread it's called from, and unbind it from any others. To use
	//	multiple contexts at the same time, users should use multiple threads and synchronise between them.

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!testEGLError("eglMakeCurrent")) {	return false; }
	return true;
}

/*!*********************************************************************************************************************
\param			eglDisplay  The EGLDisplay used by the application
\brief	Releases all resources allocated by EGL
***********************************************************************************************************************/
void releaseEGLState(EGLDisplay eglDisplay)
{
	// To release the resources in the context, first the context has to be released from its binding with the current thread.
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	// Terminate the display, and any resources associated with it (including the EGLContext)
	eglTerminate(eglDisplay);
}

/*!*********************************************************************************************************************
\param			nativeWindow                The native window to release
\param			deviceContext               The native display to release
\brief	Releases all resources allocated by the windowing system
***********************************************************************************************************************/
void releaseWindowAndDisplay(HWND nativeWindow, HDC deviceContext)
{
	// Release the device context.
	if (deviceContext) {	ReleaseDC(nativeWindow, deviceContext);	}

	// Destroy the window
	if (nativeWindow) {	DestroyWindow(nativeWindow); }
}

/*!*********************************************************************************************************************
\param			applicationInstance         Application instance created by the Operating System
\param			previousInstance            Always NULL
\param			commandLineString           Command line string passed from the Operating System, ignored.
\param			showCommand                 Specifies how the window is to be shown, ignored.
\return	Result code to send to the Operating System
\brief	Main function of the program, executes other functions.
***********************************************************************************************************************/
int main(int argc, char** argv)
{
	system("mode con lines=52 cols=160");
	HINSTANCE applicationInstance = (HINSTANCE)GetModuleHandle(NULL);

	// Windows variables
	HDC					deviceContext = NULL;

	// EGL variables
	EGLDisplay			eglDisplay = NULL;
	EGLConfig			eglConfig = NULL;
	EGLSurface			eglSurface = NULL;
	EGLContext			eglContext = NULL;

	do
	{
		// Setup the windowing system, getting a window and a display
		if (!createWindowAndDisplay(applicationInstance, g_nativeWindow, deviceContext))
			break;

		// Create and Initialize an EGLDisplay from the native display
		if (!createEGLDisplay(deviceContext, eglDisplay))
			break;

		// Choose an EGLConfig for the application, used when setting up the rendering surface and EGLContext
		if (!chooseEGLConfig(eglDisplay, eglConfig))
			break;

		// Create an EGLSurface for rendering from the native window
		if (!createEGLSurface(g_nativeWindow, eglDisplay, eglConfig, eglSurface))
			break;

		// Setup the EGL Context from the other EGL constructs created so far, so that the application is ready to submit OpenGL ES commands
		if (!setupEGLContext(eglDisplay, eglConfig, eglSurface, eglContext, g_nativeWindow))
			break;

		if (0>app_init())
			break;



		MSG msg={0};

		while(msg.message != WM_QUIT)
		{
			if(0>app_draw())
				SendMessage(g_nativeWindow, WM_QUIT, 0, 0);

			//	Present the display data to the screen.
			//	When rendering to a Window surface, OpenGL ES is double buffered. This means that OpenGL ES renders directly to one frame buffer,
			//	known as the back buffer, whilst the display reads from another - the front buffer. eglSwapBuffers signals to the windowing system
			//	that OpenGL ES 2.0 has finished rendering a scene, and that the display should now draw to the screen from the new data. At the same
			//	time, the front buffer is made available for OpenGL ES 2.0 to start rendering to. In effect, this call swaps the front and back
			//	buffers.
			if(!eglSwapBuffers(eglDisplay, eglSurface))
			{
				testEGLError("eglSwapBuffers");
				SendMessage(g_nativeWindow, WM_QUIT, 0, 0);
			}

			if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		app_destroy();
	} while(0);

	// Release the EGL State
	releaseEGLState(eglDisplay);
	// Release the windowing system resources
	releaseWindowAndDisplay(g_nativeWindow, deviceContext);
	// Destroy the eglWindow
	return 0;
}
