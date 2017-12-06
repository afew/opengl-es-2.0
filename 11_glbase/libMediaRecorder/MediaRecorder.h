
#ifndef _MediaRecorder_H_
#define _MediaRecorder_H_

#ifndef SAFE_DELETE
  #define SAFE_DELETE(v)       do{ if(v){ delete  (v); v = NULL; } }while(0);
#endif
#ifndef SAFE_DELETE_ARRAY
  #define SAFE_DELETE_ARRAY(v) do{ if(v){ delete[](v); v = NULL; } }while(0);
#endif

#include <functional>
#if !defined(_PRIMARY_WORK_)
  #include "cocos2d.h"
#endif

namespace cocos2d {

enum
{
	AVRECORDER_EVENT_OK       = 0x0000,
	AVRECORDER_EVENT_START    = 0x0001,
	AVRECORDER_EVENT_STOP     = 0x0002,
	AVRECORDER_EVENT_CANCEL   = 0x0003,
	AVRECORDER_EVENT_COMPLETE = 0x0004,
	AVRECORDER_EVENT_ETC      = 0xFFFF,
	AVRECORDER_ERR            = -1,			// error
	AVRECORDER_ERR_CONNECT    = -2,			// client did not connect
	AVRECORDER_ERR_APILEVEL   = -3,			// not support api level
	AVRECORDER_ERR_DEVICE     = -4,			// not support device
};

// media recorder
class SimpleAVRecorder
#if defined(CC_TARGET_PLATFORM)
  #if COCOS2D_VERSION < 0x00020000
    : public ::cocos2d::CCNode
  #else
    : public ::cocos2d::Node
  #endif
#endif
{
public:
	typedef std::function<void(SimpleAVRecorder* this_recorder, int event)> EVENT_FUNC;

	// create SimpleAVRecorder instance with event function
	static SimpleAVRecorder* create(const EVENT_FUNC& func_event);
	virtual ~SimpleAVRecorder(){};

	virtual int Stop() =0;					// stop recording
	virtual bool IsRecording() const =0;	// is recording
	virtual void Complete() =0;				// complete message from delegate
};

} // namespace cocos2d
#endif

