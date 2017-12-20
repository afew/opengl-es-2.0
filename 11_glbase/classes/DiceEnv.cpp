
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
	m_cube= GLTexture::createFromFile("media/texture/env/env0_file.txt", GLTexture::TYPE_CUBE);
	if(!m_cube)
		return -1;

	m_prg = GLProgram::createFromFile("media/shader/env.vert", "media/shader/env.frag");
	if(!m_prg)
		return -1;

	float cube_pos[] =
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
	float cube_nor[] =
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
	float cube_tex[] =
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
	UINT cube_idx[] =
	{
		 0,  1,  2,	 0,  2,  3,
		 4,  5,  6,	 4,  6,  7,
		 8,  9, 10,	 8, 10, 11,
		12, 13, 14,	12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	glGenBuffers(4, &mesh_cube.pos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.pos);	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos), cube_pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.nor);	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_nor), cube_nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.tex);	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_tex), cube_tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_cube.idx);	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_idx), cube_idx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// 버텍스 생성
	int	iNSphereSegments	= 128;
	sphere_triangle = 2*iNSphereSegments*(iNSphereSegments+1);

	float fDeltaRingAngle = float( LCX_PI/ iNSphereSegments );
	float fDeltaSegAngle  = float( 2.0f * LCX_PI / iNSphereSegments );

	LCXVEC3* sphere_pos = new LCXVEC3[sphere_triangle]{};
	LCXVEC3* sphere_nor = new LCXVEC3[sphere_triangle]{};
	LCXVEC2* sphere_tex = new LCXVEC2[sphere_triangle]{};

	int index=0;
	for(int seg = 0; seg < iNSphereSegments; seg++ )
	{
		float r0 = 1.0F * sinf( (seg+0) * fDeltaRingAngle );
		float r1 = 1.0F * sinf( (seg+1) * fDeltaRingAngle );
		float y0 = 1.0F * cosf( (seg+0) * fDeltaRingAngle );
		float y1 = 1.0F * cosf( (seg+1) * fDeltaRingAngle );

		for(int seg = 0; seg < (iNSphereSegments+1); seg++ )
		{
			float x0 =  r0 * sinf( seg * fDeltaSegAngle );
			float z0 =  r0 * cosf( seg * fDeltaSegAngle );
			float x1 =  r1 * sinf( seg * fDeltaSegAngle );
			float z1 =  r1 * cosf( seg * fDeltaSegAngle );

			sphere_pos[index] = LCXVEC3(x0, y0, z0);
			sphere_nor[index] = sphere_pos[index];
			sphere_tex[index] = LCXVEC2( -((float)seg)/iNSphereSegments, (seg+0)/(float)iNSphereSegments);
			++index;

			sphere_pos[index] = LCXVEC3(x1, y1, z1);
			sphere_nor[index] = sphere_pos[index];
			sphere_tex[index] = LCXVEC2( -((float)seg)/iNSphereSegments, (seg+1)/(float)iNSphereSegments);
			++index;
		}

	}

	glGenBuffers(3, &mesh_sphere.pos);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.pos);	glBufferData(GL_ARRAY_BUFFER, sizeof(LCXVEC3) *sphere_triangle, sphere_pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.nor);	glBufferData(GL_ARRAY_BUFFER, sizeof(LCXVEC3) *sphere_triangle, sphere_nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.tex);	glBufferData(GL_ARRAY_BUFFER, sizeof(LCXVEC2) *sphere_triangle, sphere_tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SAFE_DELETE_ARRAY(sphere_pos);
	SAFE_DELETE_ARRAY(sphere_nor);
	SAFE_DELETE_ARRAY(sphere_tex);

	return 0;
}

int DiceEnv::Destroy()
{
	SAFE_DELETE(m_prg);

	if(mesh_cube.pos)
	{
		glDeleteBuffers(4, &mesh_cube.pos);
		mesh_cube.pos =0;
	}
	if(mesh_sphere.pos)
	{
		glDeleteBuffers(3, &mesh_sphere.pos);
		mesh_sphere.pos =0;
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

	mtS.Scaling(4,4,4);
	mtZ.RotationZ( LCXToRadian((float)(0) ));
	mtX.RotationX( LCXToRadian((float)(c) ));
	mtY.RotationY( LCXToRadian((float)(c) ));

	mtWld = mtX* mtY;
	mtWld *= mtS;

	mtWld._41 = pos.x;
	mtWld._42 = pos.y;
	mtWld._43 = pos.z;

	// Setting Rendering pipeline
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_CULL_FACE );


	m_prg->BeginProgram();

	m_prg->Texture("us_cube", 0, m_cube);

	// get the location of uniform
	GLCamera* cam = GLCamera::globalCamera("3d world");
	if(cam)
	{
		m_prg->Vector3("uf_cam", (GLfloat*)cam->Eye());
		m_prg->Matrix16("um_Wld", (GLfloat*)&mtWld);
		m_prg->Matrix16("um_Viw", (GLfloat*)cam->View());
		m_prg->Matrix16("um_Prj", (GLfloat*)cam->Proj());
	}

	if(false)
	{
		glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.pos);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.nor);	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);	glBindBuffer(GL_ARRAY_BUFFER, mesh_cube.tex);	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_cube.idx);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.pos);	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.nor);	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);	glBindBuffer(GL_ARRAY_BUFFER, mesh_sphere.tex);	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, sphere_triangle - 2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glEnableVertexAttribArray(2);	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glEnableVertexAttribArray(1);	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glEnableVertexAttribArray(0);	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_prg->EndProgram();

	return LC_OK;
}

