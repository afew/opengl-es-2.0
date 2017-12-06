#pragma once

#ifndef _Cube_H_
#define _Cube_H_

#include "ogl_util.h"

struct Mesh
{
	UINT pos, nor, bnr, tex;	// position, normal, binormalr, texture id
	UINT idx;					// index id
};

class Dice : public RenderObject
{
protected:
	GLProgram*		m_prg  ={};
	GLTexture*		m_tx0  ={};		// diffuse
	GLTexture*		m_tx1  ={};		// normal
	Mesh			m_mesh ={};

public:
	Dice();
	virtual ~Dice();

	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
