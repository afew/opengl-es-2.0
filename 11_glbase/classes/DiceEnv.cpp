﻿
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
#include "DiceEnv.h"

static DiceEnv* g_app = NULL;

DiceEnv::DiceEnv()
{
}


DiceEnv::~DiceEnv()
{
	Destroy();
}

int DiceEnv::Init(CPVOID, CPVOID, CPVOID, CPVOID)
{
	m_tx0= GLTexture::createFromFile("media/texture/stones.tga");
	if(!m_tx0)
		return -1;

	m_tx1= GLTexture::createFromFile("media/texture/dice_5.tga");
	if(!m_tx1)
		return -1;

	m_prg = GLProgram::createFromFile("media/shader/env.vert", "media/shader/env.frag");
	if(!m_prg)
		return -1;

	float pos[] =
	{
		-1.000000F, -1.000000F, -1.000000F,
		+1.000000F, -1.000000F, -1.000000F,
		+1.000000F, -1.000000F, +1.000000F,
		-1.000000F, -1.000000F, +1.000000F,

		+1.000000F, -1.000000F, -1.000000F,
		+1.000000F, +1.000000F, -1.000000F,
		+1.000000F, +1.000000F, +1.000000F,
		+1.000000F, -1.000000F, +1.000000F,

		+1.000000F, +1.000000F, -1.000000F,
		-1.000000F, +1.000000F, -1.000000F,
		-1.000000F, +1.000000F, +1.000000F,
		+1.000000F, +1.000000F, +1.000000F,

		-1.000000F, +1.000000F, -1.000000F,
		-1.000000F, -1.000000F, -1.000000F,
		-1.000000F, -1.000000F, +1.000000F,
		-1.000000F, +1.000000F, +1.000000F,

		-1.000000F, -1.000000F, +1.000000F,
		+1.000000F, -1.000000F, +1.000000F,
		+1.000000F, +1.000000F, +1.000000F,
		-1.000000F, +1.000000F, +1.000000F,

		-1.000000F, +1.000000F, -1.000000F,
		+1.000000F, +1.000000F, -1.000000F,
		+1.000000F, -1.000000F, -1.000000F,
		-1.000000F, -1.000000F, -1.000000F,
	};
	float nor[] =
	{
		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,

		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,

		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,

		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,

		 0.000000F,  0.000000F, +1.000000F,
		 0.000000F,  0.000000F, +1.000000F,
		 0.000000F,  0.000000F, +1.000000F,
		 0.000000F,  0.000000F, +1.000000F,

		 0.000000F,  0.000000F, -1.000000F,
		 0.000000F,  0.000000F, -1.000000F,
		 0.000000F,  0.000000F, -1.000000F,
		 0.000000F,  0.000000F, -1.000000F,

	};
	float tzn[] =			// tanzent vector(x)
	{
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,

		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,
		 0.000000F, +1.000000F,  0.000000F,

		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,
		-1.000000F,  0.000000F,  0.000000F,

		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,
		 0.000000F, -1.000000F,  0.000000F,

		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,

		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,
		+1.000000F,  0.000000F,  0.000000F,

	};
	float tex[] =
	{
		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,

		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,

		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,

		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,

		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,

		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,
	};
	unsigned short idx[] =
	{
		 0,  1,  2,	 0,  2,  3,
		 4,  5,  6,	 4,  6,  7,
		 8,  9, 10,	 8, 10, 11,
		12, 13, 14,	12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	glGenBuffers(5, &m_mesh.pos);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.pos);	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.nor);	glBufferData(GL_ARRAY_BUFFER, sizeof(nor), nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.tzn);	glBufferData(GL_ARRAY_BUFFER, sizeof(tzn), tzn, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.tex);	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.idx);	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int DiceEnv::Destroy()
{
	SAFE_DELETE(m_prg);

	if(m_mesh.pos)
	{
		glDeleteBuffers(5, &m_mesh.pos);
		m_mesh.pos =0;
	}
	return 0;
}

int DiceEnv::FrameMove()
{
	return 0;
}



int	DiceEnv::Render()
{
	static float c =0;
	c += 0.5;

	// setup the world matrix
	LCXMATRIX	mtWld, mtS, mtZ, mtX, mtY;
	LCXVEC3	pos(0, 0, 0.F);

	mtS.Scaling(3,3,3);
	mtZ.RotationZ( LCXToRadian((float)(c) ));
	mtX.RotationX( LCXToRadian((float)(c) ));
	//mtY.RotationY( LCXToRadian((float)(c*4) ));

	mtWld = mtX*mtZ;
	mtWld *= mtS;

	mtWld._41 = pos.x;
	mtWld._42 = pos.y;
	mtWld._43 = pos.z;

	// Setting Rendering pipeline
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );


	m_prg->BeginProgram();

	m_prg->Texture("us_dif", 0, m_tx0);

	// get the location of uniform
	GLCamera* cam = GLCamera::globalCamera("3d world");
	if(cam)
	{
		m_prg->Matrix16("um_Wld", (GLfloat*)&mtWld);
		m_prg->Matrix16("um_Viw", (GLfloat*)cam->View());
		m_prg->Matrix16("um_Prj", (GLfloat*)cam->Proj());
	}

	glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.pos);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.tex);	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.idx);
	glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_prg->EndProgram();

	return LC_OK;
}
