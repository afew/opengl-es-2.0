#if defined(__APPLE__)

#include "MediaRecorder.h"
#include "SimpleAVRecorderIOS.h"

#import <UIKit/UIKit.h>

namespace cocos2d {

SimpleAVRecorder* SimpleAVRecorder::create(const EVENT_FUNC& event_cb)
{
	NSString* sys_verison = [[UIDevice currentDevice] systemVersion ];	// e.g. @"4.0"
	bool verion9 = ([sys_verison compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending);
	if(!verion9)	// is greater 9
		return NULL;

	SimpleAVRecorder* ret = NULL;
	if(!event_cb)
		return NULL;

	SimpleAVRecorderIOS* obj = new SimpleAVRecorderIOS;
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

}// namespace cocos2d

#endif // defined(__APPLE__)
