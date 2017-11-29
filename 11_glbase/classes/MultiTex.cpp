
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
#include "MultiTex.h"

#include "LcMath.h"

MultiTex::MultiTex()
	: m_prg(0)
	, m_tx0(0)
	, m_tx1(0)
{
}

MultiTex::~MultiTex()
{
}

int MultiTex::Init()
{
	m_prg = GLProgram::createFromFile("media/shader/multi_tex2d.vert", "media/shader/multi_tex2d.frag");
	if(!m_prg)
		return -1;

	m_tx0= GLTexture::createFromFile("media/texture/stones.tga");
	if(!m_tx0)
		return -1;
	m_tx1= GLTexture::createFromFile("media/texture/env2.tga");
	if(!m_tx1)
		return -1;
	return 0;
}

int MultiTex::Destroy()
{
	SAFE_DELETE(m_prg);
	SAFE_DELETE(m_tx0);
	SAFE_DELETE(m_tx1);
	return 0;
}

int MultiTex::FrameMove()
{
	return 0;
}

int MultiTex::Render()
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

	m_prg->BeginProgram();
	m_prg->Texture("us_tx0", 0, m_tx0);
	m_prg->Texture("us_tx1", 1, m_tx1);

	glEnableVertexAttribArray(0);	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, &Pos[0]);
	glEnableVertexAttribArray(1);	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &Tex[0]);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(1);
	m_prg->EndProgram();

	return 0;
}


