
#if defined(ANDROID)

#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>

#include <jni.h>
#include <android/log.h>

#define  LOG_TAG	"SimpleAVRecorder"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#include "SimpleAVRecorderAOS.h"

#if !defined(_PRIMARY_WORK_)
  #include "platform/android/jni/JniHelper.h"
#else
  extern int Jni_callStaticIntMethod(const char* clzz, const char* method, const char* str);
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace cocos2d {

static std::string pointer_to_string(const void* ptr_addr)
{
	std::stringstream ss;
	ss << ptr_addr;
	std::string str_addr = ss.str();
	return str_addr;
}

static void* string_to_pointer(const std::string& str_addr)
{
	void* ptr_addr = NULL;
	std::stringstream ss;
	ss <<str_addr;
	ss >>ptr_addr;
	return ptr_addr;
}

static SimpleAVRecorderAOS* g_recorder = NULL;
SimpleAVRecorderAOS::SimpleAVRecorderAOS()
 : m_recording(false)
 , m_event_cb(NULL)
{
	g_recorder = this;
}

SimpleAVRecorderAOS::~SimpleAVRecorderAOS()
{
	LOGI("SimpleAVRecorderAOS::~SimpleAVRecorderAOS() ------------------------------------------------");
	g_recorder = NULL;
}

int SimpleAVRecorderAOS::Init(const EVENT_FUNC& event_cb)
{
	std::string addr_sender = pointer_to_string(this);
	LOGI("SimpleAVRecorderAOS::Init() : this ptr: %s   -----------------------------------------------", addr_sender.c_str());
	m_event_cb   = event_cb;
#if !defined(_PRIMARY_WORK_)
	int ret = JniHelper::callStaticIntMethod(AOS_MEDIA_HELPER, "startRecording", addr_sender.c_str());
#else
	int ret = Jni_callStaticIntMethod(AOS_MEDIA_HELPER, "startRecording", addr_sender.c_str());
#endif
	return ret;
}

int SimpleAVRecorderAOS::Stop()
{
	LOGI("SimpleAVRecorderAOS::Stop():: is not supported api");
	return 0;
}

void SimpleAVRecorderAOS::Complete()
{
	if(m_event_cb)
		m_event_cb(this, AVRECORDER_EVENT_COMPLETE);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//callback from java
extern "C"
{
JNIEXPORT void
Java_gpgs_media_SimpleAVRecorder_nativeCallback (JNIEnv* env, jobject thiz, jstring jstr_addr, jint message)
{
	char* str = NULL;
	int   len = 0;
	std::string str_addr;
	if(jstr_addr)
	{
		str = (char*)env->GetStringUTFChars(jstr_addr, 0);
		len = (int  )env->GetStringUTFLength(jstr_addr);
		str_addr = str;
		env->ReleaseStringUTFChars(jstr_addr, str);
	}
	void* addr_sender = string_to_pointer(str_addr);
	LOGI("Java_kr_co_n2play_media_SimpleAVRecorder_nativeCallback:: -----------------------------------------------");
	LOGI("ptr: %p  message: %d", addr_sender, message);
	SimpleAVRecorderAOS* recorder = reinterpret_cast<SimpleAVRecorderAOS*>(addr_sender);
	if(!recorder || !recorder->m_event_cb)
	{
		LOGW("Java_kr_co_n2play_media_SimpleAVRecorder_nativeCallback:: sender object is null");
		return;
	}
	if(g_recorder != recorder)
	{
		if(!g_recorder)
		{
			LOGI("Java_kr_co_n2play_media_SimpleAVRecorder_nativeCallback:: sender already removed::  %p   %p", recorder, g_recorder);
			return;
		}
		else
		{
			LOGW("Java_kr_co_n2play_media_SimpleAVRecorder_nativeCallback:: pointer miss match::  %p   %p", recorder, g_recorder);
			return;
		}
	}
	recorder->m_recording = (AVRECORDER_EVENT_START == message);
	recorder->m_event_cb(recorder, message);
}

} // extern "C"

}// namespace cocos2d

#endif // defined(ANDROID) || defined(__ANDROID__)

