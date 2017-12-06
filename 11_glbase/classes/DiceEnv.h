#pragma once

#ifndef _DiceEnv_H_
#define _DiceEnv_H_

#include "ogl_util.h"

class DiceEnv : public RenderObject
{
protected:
	GLProgram*		m_prg  ={};
	GLTexture*		m_tx0  ={};		// diffuse
	GLTexture*		m_tx1  ={};		// normal
	GLMesh			m_mesh ={};

public:
	DiceEnv();
	virtual ~DiceEnv();

	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
