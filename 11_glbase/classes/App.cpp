
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

#include "LcMath.h"

static App* g_app = NULL;
int app_init()
{
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	LOGI("app_init() 0-------------------------------------------------------");

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
	: gl_vtx(0)
	, m_prog_tri(0)
	, m_prog_multi(0)
	, m_prog_tex(0)
	, m_tx0(0)
	, m_tx1(0)
	, m_fbo(0)
	, m_cube(0)
{
}


App::~App()
{
}

int App::Init()
{
	m_prog_tri = GLProgram::createFromFile("media/shader/simple.vert", "media/shader/simple.frag");
	if(!m_prog_tri)
		return -1;

	m_prog_multi = GLProgram::createFromFile("media/shader/multi_tex2d.vert", "media/shader/multi_tex2d.frag");
	if(!m_prog_multi)
		return -1;

	m_prog_tex = GLProgram::createFromFile("media/shader/tex2d.vert", "media/shader/tex2d.frag");
	if(!m_prog_tex)
		return -1;

	m_cube = new Dice;
	if(0> m_cube->Init())
		return -1;

	m_tx0= GLTexture::createFromFile("media/texture/stones.tga");
	if(!m_tx0)
		return -1;
	m_tx1= GLTexture::createFromFile("media/texture/env2.tga");
	if(!m_tx1)
		return -1;

	int vpt[8]={0};
	glGetIntegerv(GL_VIEWPORT, vpt);
	m_fbo = GLFBO::create(vpt[2], vpt[3]);

	LCXVECTOR3 vertexData[] =
	{
		LCXVECTOR3(-0.3f, -0.3f, 0.0f),
		LCXVECTOR3( 0.3f, -0.3f, 0.0f),
		LCXVECTOR3( 0.0f,  0.3f, 0.0f),
	};

	glGenBuffers(1, &gl_vtx);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vtx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 0;
}

int App::Destroy()
{
	if(gl_vtx) { glDeleteBuffers(1, &gl_vtx); gl_vtx = 0; }
	SAFE_DELETE(m_prog_tri);
	SAFE_DELETE(m_prog_multi);
	SAFE_DELETE(m_prog_tex);
	SAFE_DELETE(m_tx0);
	SAFE_DELETE(m_fbo);
	return 0;
}

int App::FrameMove()
{
	return 0;
}

unsigned char m_work_pixel[1024 * 1024 * 4];
int App::Render()
{
	int error;
	while((error = glGetError()) != GL_NO_ERROR);

	m_fbo->begin();
	glClearColor(0.0f, 0.4f, 0.6f, 1.0f);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// texture
	if(false)
	{
		LCXVECTOR2 Pos[] =
		{
			LCXVECTOR2(-1.0F, +0.0F),
			LCXVECTOR2( 0.0F, +0.0F),
			LCXVECTOR2( 0.0F, +1.0F),
			LCXVECTOR2(-1.0F, +1.0F),
		};
		LCXVECTOR2 Tex[] =
		{
			LCXVECTOR2(0.0F, 0.0F),
			LCXVECTOR2(1.0F, 0.0F),
			LCXVECTOR2(1.0F, 1.0F),
			LCXVECTOR2(0.0F, 1.0F),
		};

		m_prog_multi->BeginProgram();

		m_prog_multi->Texture("us_tx0", 0, m_tx0);
		m_prog_multi->Texture("us_tx1", 1, m_tx1);

		glEnableVertexAttribArray(0);	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, &Pos[0]);
		glEnableVertexAttribArray(1);	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &Tex[0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(1);
		m_tx1->UnBindStage(1);
		m_tx0->UnBindStage(0);
		m_prog_multi->EndProgram();
	}

	// triangle
	if(false)
	{
		m_prog_tri->BeginProgram();
		static int angle =0;
		++angle;
		if(angle>360)
			angle = 0;

		float r= float(angle * 3.14159265358979 /180.0);
		const float transformationMatrix[] =
		{
			+cosf(r), -sinf(r), 0.0f, 0.0f,
			+sinf(r), +cosf(r), 0.0f, 0.0f,
			    0.0f,     0.0f, 1.0f, 0.0f,
			    0.0f,     0.0f, 0.0f, 1.0f
		};

		LCXMATRIX tmWld;

		tmWld.RotationZ(r);
		if(0>m_prog_tri->Matrix16("um_tmWorld", tmWld))
		{
			return -1;
		}

		static int dir = +1;
		static int cc = 120;
		static int gr = 120;

		cc += dir;
		if(cc>255)
		{
			dir = -1;
			cc = 255;

			gr = 100 + rand()%156;
		}
		if(cc<120)
		{
			dir = 1;
			cc = 120;
			gr = 100 + rand()%156;
		}

		float color[] ={cc/255.0F, gr/255.0F, cc/255.0F, 1.0F};
		if(0>m_prog_tri->Color4("uc_dif", color))
		{
			return -1;
		}

		glBindBuffer(GL_ARRAY_BUFFER, gl_vtx);
		glEnableVertexAttribArray(0);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_prog_tri->EndProgram();
	}

	
		m_cube->FrameMove();
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


