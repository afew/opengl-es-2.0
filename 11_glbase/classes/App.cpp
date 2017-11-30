
#include <mutex>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#import <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#endif

#include "app_util.h"
#include "App.h"
#include "Dice.h"
#include "MultiTex.h"
#include "LcMath.h"

static std::mutex g_mutex;

static App* g_app = NULL;

struct Touch{ int a; float x, y; };	// action, position
static unsigned char g_key_event[256]={0};
static unsigned char g_key_cur  [256]={0};
static Touch g_touch_event[32]={0};
static Touch g_touch_cur  [32]={0};

void app_key_event(int index, int action)
{
	std::lock_guard<std::mutex> lock(g_mutex);

	if(0>index || 255<index)
		return;
	g_key_event[index]=action;
}

void app_touch_event(int index, int action, float x, float y)
{
	std::lock_guard<std::mutex> lock(g_mutex);
	if(0>index || 32<index)
		return;

	//LOGI("app_touch_event: index: %3d  action: %2d ", index, action);
	g_touch_event[index].a = action;
	g_touch_event[index].x = x;
	g_touch_event[index].y = y;
}

void app_update_key()
{
	std::lock_guard<std::mutex> lock(g_mutex);

	memcpy(g_key_cur  , g_key_event  , sizeof(g_key_event  ));
	memcpy(g_touch_cur, g_touch_event, sizeof(g_touch_event));

	memset(g_key_event  , 0, sizeof(g_key_event  ));
	memset(g_touch_event, 0, sizeof(g_touch_event));
}

int app_init()
{
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	LOGI("----------------------------------------------------------------------");

	return App::getInstance()->Init();
}

int app_destroy()
{
	if(g_app)
	{
		return App::getInstance()->Destroy();
		delete g_app;
		g_app = NULL;
	}
	return 0;
}

int app_update()
{
	return App::getInstance()->FrameMove();
}

int app_draw()
{
	return App::getInstance()->Render();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

App* App::getInstance()
{
	if(!g_app)
		g_app = new App;
	return g_app;
}

App::App()
	: m_fbo  {}
	, m_cube {}
	, m_multi{}
	, m_cam  {}
{
}


App::~App()
{
}

int App::Init()
{
	m_cam = GLCamera::create();
	if(!m_cam)
		return -1;

	GLCamera::globalCamera(std::string("3d world"), m_cam);

	m_cube = new Dice;
	if(0> m_cube->Init())
		return -1;

	m_multi = new MultiTex;
	if(0> m_multi->Init())
		return -1;

	int vpt[8]={0};
	glGetIntegerv(GL_VIEWPORT, vpt);
	m_fbo = GLFBO::create(vpt[2], vpt[3]);

	return 0;
}

int App::Destroy()
{
	SAFE_DELETE(m_fbo  );
	SAFE_DELETE(m_cube );
	SAFE_DELETE(m_multi);
	SAFE_DELETE(m_cam  );
	return 0;
}

int App::FrameMove()
{
	if(1 == g_touch_cur[0].a)
	{
		LOGI("App::FrameMove() touch -------------------------------------------------------");
	}
	m_cam->FrameMove();
	m_multi->FrameMove();
	m_cube->FrameMove();
	return 0;
}

int App::Render()
{
		int error;
	while((error = glGetError()) != GL_NO_ERROR);

	//m_fbo->begin();
	glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	m_multi->Render();
	glClear(GL_DEPTH_BUFFER_BIT);
	m_cube->Render();

	//m_fbo->end();
	//glReadPixels(0, 0, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, m_work_pixel);
	//m_fbo->SetPixel(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, m_work_pixel);

	if(false)
	{
		glClearColor(0.4f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_fbo->draw();
	}
	return 0;
}


