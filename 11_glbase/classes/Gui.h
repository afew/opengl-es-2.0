#pragma once

#ifndef _Gui_H_
#define _Gui_H_

#include "ogl_util.h"

class GuiObject: public RenderObject
{
};

class Gui : public GuiObject
{
protected:
	GLProgram*		m_prg {};
	GLTexture*		m_tx0 {};
	GLTexture*		m_tx1 {};

public:
	Gui();
	virtual ~Gui();

	static GuiObject* createButton(const char* file_name0, const char* file_name1);
	virtual int Init      (CPVOID =NULL, CPVOID =NULL, CPVOID =NULL, CPVOID =NULL) override;
	virtual int Destroy   () override;
	virtual int FrameMove () override;
	virtual int Render    () override;
};

#endif
