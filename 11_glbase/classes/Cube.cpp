
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
#include "Cube.h"

#include "LcMath.h"

static Cube* g_app = NULL;

Cube::Cube()
	: m_prog_3d(0)
{
}


Cube::~Cube()
{
	Destroy();
}

int Cube::Init()
{
	m_prog_3d = GLProgram::createFromFile("media/shader/simple3d.vert", "media/shader/simple3d.frag");
	if(!m_prog_3d)
		return -1;

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
	LCXVECTOR3	eye(0, 60, 10);
	LCXVECTOR3	look(0,0,0);
	LCXVECTOR3	up(0,0,1);
	m_mtViw.ViewGl(&eye, &look, &up);
	return 0;
}

int Cube::Destroy()
{
	SAFE_DELETE(m_prog_3d);
	return 0;
}

int Cube::FrameMove()
{
	return 0;
}

// Rendering vertex data
const float len= 10.0F;
static float pCube[] =
{
	-len,-len,-len,  +len,-len,-len, +len,+len,-len, -len,+len,-len,
	-len,-len,+len,  +len,-len,+len, +len,+len,+len, -len,+len,+len
};

//      7 +----------+ 6
//       /|         /|
//      / |        / |
//     /  |       /  |
//  3 +----------+ 2 |
//    |   |      |   |
//    | 4 +------|---+ 5
//    |  /       |  /
//    | /        | /
//    |/         |/
//  0 +----------+ 1

static WORD front[]  = {2,1,3,0};
static WORD back[]   = {5,6,4,7}; //back face
static WORD top[]    = {6,2,7,3}; //top face
static WORD bottom[] = {1,5,0,4}; //bottom face
static WORD left[]   = {3,0,7,4}; //left face
static WORD right[]  = {6,5,2,1}; //right face


int	Cube::Render()
{
	static int c =0;
	++c;

	// setup the world matrix
	LCXMATRIX	mtWld, mtZ, mtX, mtY;
	LCXVECTOR3	pos(0, 0, 0.F);

	mtZ.RotationZ( LCXToRadian((float)(c) ));
	mtX.RotationX( LCXToRadian((float)(c) ));
	mtY.RotationY( LCXToRadian((float)(c*2) ));
	mtWld = mtZ * mtX * mtY;

	mtWld._41 = pos.x;
	mtWld._42 = pos.y;
	mtWld._43 = pos.z;

	// Setting Rendering pipeline
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );

	m_prog_3d->BeginProgram();

	// get the location of uniform
	m_prog_3d->Matrix16("um_Wld", (GLfloat*)&  mtWld);
	m_prog_3d->Matrix16("um_Viw", (GLfloat*)&m_mtViw);
	m_prog_3d->Matrix16("um_Prj", (GLfloat*)&m_mtPrj);


	//Enable the vertices array
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, pCube);


	LCXCOLOR v1(1,0,0,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v1.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, front);
	LCXCOLOR v2(0,1,0,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v2.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, back);
	LCXCOLOR v3(0,0,1,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v3.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, top);
	LCXCOLOR v4(1,1,0,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v4.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, bottom);
	LCXCOLOR v5(0,1,1,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v5.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, left);
	LCXCOLOR v6(1,0,1,1);	m_prog_3d->Color4("uc_dif", (GLfloat*)&v6.m[0]);	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, right);

	m_prog_3d->EndProgram();

	return LC_OK;
}

