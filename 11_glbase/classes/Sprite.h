#pragma once

#ifndef _Sprite_H_
#define _Sprite_H_

#include "ogl_util.h"

class Sprite : public RenderObject
{
protected:
	GLProgram*		m_prg {};
	GLTexture*		m_tx0 {};
	GLTexture*		m_tx1 {};

public:
	Sprite();
	virtual ~Sprite();

	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
