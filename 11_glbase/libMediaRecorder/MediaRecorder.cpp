#if defined(ANDROID) || defined(__ANDROID__) || defined(_WIN32)

#include "MediaRecorder.h"

#if defined(ANDROID) || defined(__ANDROID__)
  #include "SimpleAVRecorderAOS.h"
#elif defined(_WIN32)
  #include "SimpleAVRecorderWin32.h"
#endif

#if !defined(_PRIMARY_WORK_) && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  #include "platform/android/jni/JniHelper.h"
#elif defined(ANDROID) || defined(__ANDROID__)
  extern long long Jni_callStaticLongMethod(const char* clzz, const char* method);
#endif

namespace cocos2d {

SimpleAVRecorder* SimpleAVRecorder::create(const EVENT_FUNC& event_cb)
{
	SimpleAVRecorder* ret = NULL;
	if(!event_cb)
		return NULL;

#if defined(ANDROID) || defined(__ANDROID__)
	SimpleAVRecorderAOS* obj = new SimpleAVRecorderAOS;
#elif defined(_WIN32)
	SimpleAVRecorderWin32* obj = new SimpleAVRecorderWin32;
#endif
	if(0 > obj->Init(event_cb))
	{
		delete obj;
		return NULL;
	}
	ret = obj;
#if defined(CC_TARGET_PLATFORM)
	ret->autorelease();
#endif
	return ret;
}

} // namespace cocos2d

#endif // defined(ANDROID) || defined(__ANDROID__) || defined(_WIN32)

