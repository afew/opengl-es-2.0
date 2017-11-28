#pragma once

#ifndef _Cube_H_
#define _Cube_H_

#include "ogl_util.h"
#include "LcType.h"
#include "LcMath.h"

struct Mesh
{
	UINT pos, nor, tex;		// position, normal, texture id
	UINT idx;				// index id
};

class Dice
{
protected:
	GLProgram*		m_prg  ={};
	GLTexture*		m_tx0  ={};		// diffuse
	GLTexture*		m_tx1  ={};		// normal
	Mesh			m_mesh ={};

	LCXMATRIX		m_mtViw;
	LCXMATRIX		m_mtPrj;

public:
	Dice();
	virtual ~Dice();

	virtual int Init();
	virtual int Destroy();
	virtual int FrameMove();
	virtual int Render();
};

#endif
