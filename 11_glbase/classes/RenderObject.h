#pragma once

#ifndef _RenderObject_H_
#define _RenderObject_H_

struct RenderObject
{
	virtual ~RenderObject(){};
	virtual int Init() =0;
	virtual int Destroy() =0;
	virtual int FrameMove() =0;
	virtual int Render() =0;
};

#endif
