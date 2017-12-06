#if defined(_WIN32)
#pragma once

#ifndef _SimpleAVRecorderWin32_H_
#define _SimpleAVRecorderWin32_H_

#include "MediaRecorder.h"

#ifndef OVERRIDE
  #if 1700 <= _MSC_VER
    #define OVERRIDE override
  #else
    #define OVERRIDE
  #endif
#endif

namespace cocos2d {

class SimpleAVRecorderWin32 : public SimpleAVRecorder
{
public:
	SimpleAVRecorderWin32();
	virtual ~SimpleAVRecorderWin32();

	virtual int Stop() OVERRIDE { return AVRECORDER_ERR; }
	virtual bool IsRecording() const OVERRIDE{return false;}
	virtual void Complete() OVERRIDE {};

	int  Init(const EVENT_FUNC& event_cb);
};

}// namespace cocos2d

#endif

#endif // defined(_WIN32)

