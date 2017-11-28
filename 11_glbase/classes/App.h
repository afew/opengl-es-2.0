#pragma once

#ifndef _App_H_
#define _App_H_

#include "ogl_util.h"

class App
{
protected:
	GLProgram*		m_prog_tri;
	GLProgram*		m_prog_multi;
	GLProgram*		m_prog_tex;
	GLTexture*		m_tx0;
	GLTexture*		m_tx1;
	GLFBO	*		m_fbo;
	unsigned int	gl_vtx;

public:
	static App* getInstance();
	App();
	virtual ~App();

	virtual int Init();
	virtual int Destroy();
	virtual int FrameMove();
	virtual int Render();
};

#endif
