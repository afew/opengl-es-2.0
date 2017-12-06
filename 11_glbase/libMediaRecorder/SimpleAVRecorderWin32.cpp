
#if defined(_WIN32)

#include <string>
#include <stdio.h>

#include "SimpleAVRecorderWin32.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace cocos2d {

SimpleAVRecorderWin32::SimpleAVRecorderWin32()
{
}

SimpleAVRecorderWin32::~SimpleAVRecorderWin32()
{
}

int SimpleAVRecorderWin32::Init(const EVENT_FUNC& event_cb)
{
	return -1;
}

}// namespace cocos2d

#endif // defined(_WIN32)

