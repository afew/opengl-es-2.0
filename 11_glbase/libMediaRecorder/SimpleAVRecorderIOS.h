#if defined(__APPLE__)

#ifndef _SimpleAVRecorderIOS_H_
#define _SimpleAVRecorderIOS_H_

#include "MediaRecorder.h"

#ifndef OVERRIDE
  #if 201103L <= __cplusplus
    #define OVERRIDE override
  #else
    #define OVERRIDE
  #endif
#endif

namespace cocos2d {

class SimpleAVRecorderIOS : public SimpleAVRecorder
{
public:
	SimpleAVRecorderIOS();
	virtual ~SimpleAVRecorderIOS(){};
	virtual int Stop() OVERRIDE;
	virtual bool IsRecording() const OVERRIDE { return m_recording; }
	virtual void Complete() OVERRIDE;

	int  Init(const EVENT_FUNC& event_cb);

protected:
	bool m_recording;		// is recording
	EVENT_FUNC m_event_cb;	// event callback
};

} // namespace cocos2d

#endif

#endif // defined(__APPLE__)

