#pragma once

#ifndef _App_H_
#define _App_H_

#include "ogl_util.h"

class App : public RenderObject
{
protected:
	GLFBO*			m_fbo   ={};
	RenderObject*	m_cube  ={};
	RenderObject*	m_multi ={};
	RenderObject*	m_cam   ={};

public:
	static App* getInstance();
	App();
	virtual ~App();

	virtual int Init() override;
	virtual int Destroy() override;
	virtual int FrameMove() override;
	virtual int Render() override;
};

#endif
