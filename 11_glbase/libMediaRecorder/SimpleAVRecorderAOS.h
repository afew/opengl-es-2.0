#if defined(ANDROID) || defined(__ANDROID__)

#ifndef _SimpleAVRecorderAOS_H_
#define _SimpleAVRecorderAOS_H_

#include "MediaRecorder.h"

#ifndef OVERRIDE
  #if 201103L <= __cplusplus
    #define OVERRIDE override
  #else
    #define OVERRIDE
  #endif
#endif

#define AOS_MEDIA_HELPER	"gpgs/media/SimpleAVRecorder"

namespace cocos2d {

class SimpleAVRecorderAOS : public SimpleAVRecorder
{
public:
	SimpleAVRecorderAOS();
	virtual ~SimpleAVRecorderAOS();

	virtual int Stop() OVERRIDE;
	virtual bool IsRecording() const OVERRIDE { return m_recording; }
	virtual void Complete() OVERRIDE;

	int Init(const EVENT_FUNC& event_cb);

public:
	bool m_recording;			// is recording
	EVENT_FUNC m_event_cb;		// event callback
};

} // namespace cocos2d

#endif

#endif // defined(ANDROID) || defined(__ANDROID__)

