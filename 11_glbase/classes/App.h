﻿#pragma once

#ifndef _App_H_
#define _App_H_

#include "ogl_util.h"
#include "LcType.h"
#include "LcMath.h"


class App : public RenderObject
{
protected:
	GLFBO*			m_fbo;
	RenderObject*	m_cube ={};
	RenderObject*	m_multi ={};

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
