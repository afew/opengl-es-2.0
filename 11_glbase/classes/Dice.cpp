
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
#include "Dice.h"

#include "LcMath.h"

static Dice* g_app = NULL;

Dice::Dice()
{
}


Dice::~Dice()
{
	Destroy();
}

int Dice::Init()
{
	m_tx0= GLTexture::createFromFile("media/texture/metal.tga");
	if(!m_tx0)
		return -1;

	m_tx1= GLTexture::createFromFile("media/texture/dice_5.tga");
	if(!m_tx1)
		return -1;

	m_prg = GLProgram::createFromFile("media/shader/dice.vert", "media/shader/dice.frag");
	if(!m_prg)
		return -1;

	float pos[] =
	{
		 1.000000F,  1.000000F, -1.000000F,
		-1.000000F,  1.000000F, -1.000000F,
		-1.000000F,  1.000000F,  1.000000F,
		 1.000000F,  1.000000F,  1.000000F,
		 1.000000F, -1.000000F, -1.000000F,
		-1.000000F, -1.000000F, -1.000000F,
		-1.000000F, -1.000000F,  1.000000F,
		 1.000000F, -1.000000F,  1.000000F,
	};
	float nor[] =
	{
		 0.333333F,  0.666667F, -0.666667F,
		-0.816497F,  0.408248F, -0.408248F,
		-0.333333F,  0.666667F,  0.666667F,
		 0.816497F,  0.408248F,  0.408248F,
		 0.666667F, -0.666667F, -0.333333F,
		-0.408248F, -0.408248F, -0.816497F,
		-0.666667F, -0.666667F,  0.333333F,
		 0.408248F, -0.408248F,  0.816497F,
	};
	float tex[] =
	{
		0.000000F, 1.000000F,
		1.000000F, 1.000000F,
		0.000000F, 1.000000F,
		1.000000F, 1.000000F,
		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
		0.000000F, 0.000000F,
		1.000000F, 0.000000F,
	};
	unsigned short idx[] =
	{
		0, 1, 2,	0, 2, 3,
		0, 4, 5,	0, 5, 1,
		1, 5, 6,	1, 6, 2,
		2, 6, 7,	2, 7, 3,
		3, 7, 4,	3, 4, 0,
		4, 7, 6,	4, 6, 5,
	};

	glGenBuffers(4, &m_mesh.pos);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.pos);	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.nor);	glBufferData(GL_ARRAY_BUFFER, sizeof(nor), nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.tex);	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.idx);	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// set up projection matrix in 3d pipeline
	float	vpt[16]={0};
	glGetFloatv(GL_VIEWPORT, vpt);		// get viewport to set the screen width and height.
	float scnW = vpt[2];
	float scnH = vpt[3];


	float	Asp = scnW/scnH;
	float	Near = 1.F;
	float	Far	 = 1000.F;
	float	Fov  = 45.F;
	m_mtPrj.PerspectiveD3dRH( LCXToRadian(Fov), Asp, Near, Far);


	// setup the view matrix
	LCXVECTOR3	eye(0, -20, 0);
	LCXVECTOR3	look(0,0,0);
	LCXVECTOR3	up(0,0,1);
	m_mtViw.ViewGl(&eye, &look, &up);
	return 0;
}

int Dice::Destroy()
{
	SAFE_DELETE(m_prg);

	if(m_mesh.pos)
	{
		glDeleteBuffers(4, &m_mesh.pos);
		m_mesh.pos =0;
	}
	return 0;
}

int Dice::FrameMove()
{
	return 0;
}



int	Dice::Render()
{
	static int c =0;
	++c;

	// setup the world matrix
	LCXMATRIX	mtWld, mtS, mtZ, mtX, mtY;
	LCXVECTOR3	pos(0, 0, 0.F);

	mtS.Scaling(3,3,3);
	mtZ.RotationZ( LCXToRadian((float)(c) ));
	mtX.RotationX( LCXToRadian((float)(c) ));
	mtY.RotationY( LCXToRadian((float)(c*2) ));
	mtWld = mtX*mtZ;
	mtWld *= mtS;

	mtWld._41 = pos.x;
	mtWld._42 = pos.y;
	mtWld._43 = pos.z;

	// Setting Rendering pipeline
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );


	LCXVECTOR3 lgt_dir(  -0.707F, -0.707F, 0);
	LCXCOLOR   lgt_dif(1.8F,   0.6F, 1.2F, 1.0F);

	LCXVECTOR3	eye(0, -20, 0);
	LCXVECTOR3	look(0,0,0);


	m_prg->BeginProgram();

	m_prg->Vector3("uf_cam", eye);
	m_prg->Vector3("lgt_dir", lgt_dir);
	m_prg->Color4 ("lgt_dif", lgt_dif);


	m_prg->Texture("us_dif", 0, m_tx0);
	m_prg->Texture("us_nor", 1, m_tx1);


	// get the location of uniform
	m_prg->Matrix16("um_Wld", (GLfloat*)&  mtWld);
	m_prg->Matrix16("um_Viw", (GLfloat*)&m_mtViw);
	m_prg->Matrix16("um_Prj", (GLfloat*)&m_mtPrj);


	glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.pos);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.nor);	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);	glBindBuffer(GL_ARRAY_BUFFER, m_mesh.tex);	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.idx);
	glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(2);	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_prg->EndProgram();

	return LC_OK;
}

