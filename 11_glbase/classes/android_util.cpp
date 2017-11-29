#if defined(ANDROID) || defined(__ANDROID__)

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "app_util.h"

JavaVM* jni_java_jvm = NULL;							// JVM instance

static AAssetManager* g_assm = NULL;
void setAAssetManager(void* assm)
{
	g_assm = (AAssetManager*)assm;
}

void* getAAssetManager() { return g_assm; }

const char* AppResourcePath(const char* fileName)
{
	return fileName;
}

int find_java_static_method(JNIEnv**   ret_env
						 , jclass*	ret_cls
						 , jmethodID* ret_mid
						 , const char* clzz, const char* method, const char* sig)
{
	JNIEnv*	env	= NULL;
	JavaVM*	jvm	= jni_java_jvm;

	LOGI("find_java_static_method 0 :: %s :: %s ", clzz, method);
	if(NULL == jvm || 0 > jvm->AttachCurrentThread(&env, NULL))
	{
		LOGE("Failed::find_java_satic_method::AttachCurrentThread error");
		return NSDK_EFAIL;
	}

	if(NULL == env)
	{
		LOGE("Failed::find_java_satic_method::env null");
		return NSDK_EFAIL;
	}

	LOGI("find_java_static_method 1 :: FindClass");
	jclass clz = env->FindClass(clzz);
	if(NULL == clz)
	{
		LOGE("Failed::find_java_satic_method::find class::%s error", clzz);
		return NSDK_EFAIL;
	}

	LOGI("find_java_static_method 2:: GetStaticMethodID:: %s", sig);
	jmethodID mid = env->GetStaticMethodID(clz, method, sig);
	if(NULL == mid)
	{
		LOGE("Failed::find_java_satic_method::get method::%s %s error", clzz, method);
		return NSDK_EFAIL;
	}

	*ret_env = env;
	*ret_cls = clz;
	*ret_mid = mid;
	return NSDK_OK;
}

int find_java_static_field(JNIEnv**  ret_env
						, jclass*   ret_cls
						, jfieldID* ret_fid
						, const char* clzz, const char* field, const char* sig)
{
	JNIEnv*	env	= NULL;
	JavaVM*	jvm	= jni_java_jvm;
	if(NULL == jvm || 0 > jvm->AttachCurrentThread(&env, NULL))
	{
		LOGE("Failed::find_java_satic_field::AttachCurrentThread error");
		return NSDK_EFAIL;
	}
	if(NULL == env)
	{
		LOGE("Failed::find_java_satic_field::env null");
		return NSDK_EFAIL;
	}
	jclass clz = env->FindClass(clzz);
	if(NULL == clz)
	{
		LOGE("Failed::find_java_satic_field::find class::%s error", clzz);
		return NSDK_EFAIL;
	}
	jfieldID fid = env->GetStaticFieldID(clz, field, sig);
	if(NULL == fid)
	{
		LOGE("Failed::find_java_satic_field::get field:: %s : %s error", clzz, field);
		return NSDK_EFAIL;
	}
	*ret_env = env;
	*ret_cls = clz;
	*ret_fid = fid;
	return NSDK_OK;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

long long Jni_callStaticLongMethod(const char* clzz, const char* method)
{
	JNIEnv* env = NULL; jclass clz ={}; jmethodID mid ={};
	int hr = find_java_static_method(&env, &clz, &mid, clzz, method, "()J");
	if(0 > hr)
	{
		LOGE("JNI::find method:: [%3d] %s:", __LINE__, __func__);
		return NSDK_EFAIL;
	}
	long long ret = env->CallStaticLongMethod(clz, mid);
	LOGI("Jni_callStaticLongMethod: %lld", ret);
	return ret;
}

int Jni_callStaticVoidMethod(const char* clzz, const char* method)
{
	JNIEnv* env = NULL; jclass clz ={}; jmethodID mid ={};
	int hr = find_java_static_method(&env, &clz, &mid, clzz, method, "()V");
	if(0 > hr)
	{
		LOGE("JNI::find method:: [%3d] %s:", __LINE__, __func__);
		return NSDK_EFAIL;
	}
	env->CallStaticVoidMethod(clz, mid);
	return hr;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// declaration jni function
extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);
}	// extern "C"


// define jni function
jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	jint	result = -1;
	JNIEnv*	env = NULL;
	jni_java_jvm = jvm;

	LOGW("JNI_OnLoad----------------------");

	// test..
	if(jvm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	{
		LOGE("GetEnv failed!---------------");
		return result;
	}

	if(!env)
		LOGE("Could not retrieve the env!---------------");

	jni_java_jvm = jvm;
	return JNI_VERSION_1_6;
}


#endif // defined(ANDROID) || defined(__ANDROID__)

