#pragma once

#ifndef _Dice_H_
#define _Dice_H_

#include "ogl_util.h"

class Dice : public RenderObject
{
protected:
	GLProgram*		m_prg  {};
	GLTexture*		m_tx0  {};		// diffuse
	GLTexture*		m_tx1  {};		// normal
	GLTexture*		m_cube {};		// diffuse
	GLMesh			m_mesh {};

public:
	Dice();
	virtual ~Dice();

	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
