#pragma once

#ifndef _Cube_H_
#define _Cube_H_

#include "ogl_util.h"
#include "LcType.h"
#include "LcMath.h"

class Cube
{
protected:
	GLProgram*		m_prog_3d;
	LCXMATRIX		m_mtViw;
	LCXMATRIX		m_mtPrj;

public:
	Cube();
	virtual ~Cube();

	virtual int Init();
	virtual int Destroy();
	virtual int FrameMove();
	virtual int Render();
};

#endif
