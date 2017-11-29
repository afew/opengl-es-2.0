#pragma once

#ifndef _MultiTex_H_
#define _MultiTex_H_

#include "ogl_util.h"
#include "LcType.h"
#include "LcMath.h"

class MultiTex : public RenderObject
{
protected:
	GLProgram*		m_prg;
	GLTexture*		m_tx0;
	GLTexture*		m_tx1;

public:
	MultiTex();
	virtual ~MultiTex();

	virtual int Init() override;
	virtual int Destroy() override;
	virtual int FrameMove() override;
	virtual int Render() override;
};

#endif
