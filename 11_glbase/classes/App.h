#pragma once

#ifndef _App_H_
#define _App_H_

#include "ogl_util.h"
#include "Gui.h"

class App : public RenderObject
{
protected:
	GLFBO*			m_fbo    {};
	RenderObject*	m_dice   {};
	GuiObject*		m_button {};
	GLCamera*		m_cam3d  {};
	GLCamera*		m_cam_gui{};

public:
	static App* getInstance();
	App();
	virtual ~App();

	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
