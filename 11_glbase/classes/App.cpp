
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

#include "MediaRecorder.h"
using namespace cocos2d;


static std::mutex g_mutex;

static App* g_app = NULL;

struct Touch{ int a; float x, y; };	// action, position
static unsigned char g_key_event[256]={0};
static unsigned char g_key_cur  [256]={0};
static Touch g_touch_event[32]={0};
static Touch g_touch_cur  [32]={0};

static bool test_touch_rect(int touch_index, int touch_action, const LCXRECT& rc)
{
	const Touch* touch = &g_touch_cur[touch_index];
	if(touch_action != touch->a)
		return false;

	if(	touch->x > rc.x          &&
		touch->x < (rc.x + rc.w) &&
		touch->y > rc.y          &&
		touch->y < (rc.y + rc.h) )
	{
		return true;
	}
	return false;
}

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

void app_update_input()
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
	//printGLString("Extensions", GL_EXTENSIONS);
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
	app_update_input();
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
	, m_cam  {}
	, m_button{}
{
}


App::~App()
{
}

int App::Init(CPVOID, CPVOID, CPVOID, CPVOID)
{
	m_cam = GLCamera::create();
	if(!m_cam)
		return -1;
	GLCamera::globalCamera(std::string("3d world"), m_cam);

	m_cube = new Dice;
	if(0> m_cube->Init())
		return -1;

	m_button = Gui::createButton("media/texture/stones.tga", "media/texture/white.tga");
	if(!m_button)
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
	SAFE_DELETE(m_cam  );
	SAFE_DELETE(m_button);
	return 0;
}

int App::FrameMove()
{
	if(1 == g_touch_cur[0].a)
	{
		LOGI("App::FrameMove() touch -------------------------------------------------------");
	}
	m_cam->FrameMove();
	m_button->FrameMove();
	m_cube->FrameMove();

	if(test_touch_rect(0, 1, LCXRECT(0,0, 300, 200)))
	{
		SimpleAVRecorder* simple_recorder = ::cocos2d::SimpleAVRecorder::create([this](cocos2d::SimpleAVRecorder* obj, int event)
		{
			if(!obj)
				return;
			#if defined(ANDROID) || defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "SimpleAVRecorder", "App::FrameMove():: event code :: ", event);
			#endif
			switch(event)
			{
				case AVRECORDER_EVENT_START:
				{
#if defined(CC_TARGET_PLATFORM)
					Node* pChild = this->getChildByTag(START_VIDEO_RECORDING);
					if(pChild)
						pChild->setVisible(false);
					pChild = this->getChildByTag(STOP_VIDEO_RECORDING);
					if(pChild)
					{
						pChild->setVisible(true);
					}
#else
					#if defined(ANDROID) || defined(__ANDROID__)
					__android_log_print(ANDROID_LOG_INFO, "SimpleAVRecorder", "App::FrameMove():: simple_recorder:: event receive: start");
					#endif
#endif
					break;
				}
				case AVRECORDER_EVENT_STOP:
				{
#if defined(CC_TARGET_PLATFORM)
					Node* pChild = this->getChildByTag(START_VIDEO_RECORDING);
					if(pChild)
						pChild->setVisible(true);
					pChild = this->getChildByTag(STOP_VIDEO_RECORDING);
					if(pChild)
						pChild->setVisible(false);
#else
					#if defined(ANDROID) || defined(__ANDROID__)
					__android_log_print(ANDROID_LOG_INFO, "SimpleAVRecorder", "App::FrameMove():: simple_recorder:: event receive: stop");
					#endif
#endif
					break;
				}
				case AVRECORDER_EVENT_COMPLETE:
				{
#if defined(CC_TARGET_PLATFORM)
					Node* pChild = this->getChildByTag(START_VIDEO_RECORDING);
					if(pChild)
						pChild->setVisible(true);
					pChild = this->getChildByTag(STOP_VIDEO_RECORDING);
					if(pChild)
						pChild->setVisible(false);

					obj->removeFromParentAndCleanup(true);
#else
					delete obj;
					#if defined(ANDROID) || defined(__ANDROID__)
					__android_log_print(ANDROID_LOG_INFO, "SimpleAVRecorder", "App::FrameMove():: simple_recorder:: event receive: complete");
					#endif
#endif
					break;
				}
				default:
				{
					break;
				}
			}
		});
#if defined(CC_TARGET_PLATFORM)
		if(simple_recorder)
		{
			simple_recorder->setTag(0x7FFF0011);
			addChild(simple_recorder, 0x7FFF0000);	//  가장 늦게 방문하도록 함
		}
#endif
	}
	return 0;
}

int App::Render()
{
	//int error;
	//while((error = glGetError()) != GL_NO_ERROR);

	m_fbo->begin();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_button->Render();
	glClear(GL_DEPTH_BUFFER_BIT);
	m_cube->Render();

	m_fbo->end();
	//glReadPixels(0, 0, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, m_work_pixel);
	//m_fbo->SetPixel(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, m_work_pixel);

	if(true)
	{
		glClearColor(0.4f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_fbo->draw();
	}
	return 0;
}


