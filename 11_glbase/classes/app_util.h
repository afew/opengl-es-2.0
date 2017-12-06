#pragma once

#ifndef _APP_UTIL_H_
#define _APP_UTIL_H_

#define SAFE_DELETE(v)       { if(v){ delete  (v); v = NULL; } }
#define SAFE_DELETE_ARRAY(v) { if(v){ delete[](v); v = NULL; } }

#ifndef __func__
  #define __func__ __FUNCTION__
#endif
#ifndef __FUNCTION__
  #define __FUNCTION__ __func__
#endif

#if defined(_WIN32) || defined(__APPLE__)
	typedef unsigned char   jboolean;	// unsigned 8 bits
	typedef signed char     jbyte   ;	// signed 8 bits
	typedef unsigned short  jchar   ;	// unsigned 16 bits
	typedef short           jshort  ;	// signed 16 bits
	typedef int             jint    ;	// signed 32 bits
	typedef long long       jlong   ;	// signed 64 bits
	typedef float           jfloat  ;	// 32-bit IEEE 754
	typedef double          jdouble ;	// 64-bit IEEE 754
	typedef jint            jsize   ;

	class _jobject {};							class _jclass       : public _jobject{};	class _jstring      : public _jobject{};
	class _jarray       : public _jobject{};	class _jobjectArray : public _jarray {};	class _jbooleanArray: public _jarray {};
	class _jbyteArray   : public _jarray {};	class _jcharArray   : public _jarray {};	class _jshortArray  : public _jarray {};
	class _jintArray    : public _jarray {};	class _jlongArray   : public _jarray {};	class _jfloatArray  : public _jarray {};
	class _jdoubleArray : public _jarray {};	class _jthrowable   : public _jobject{};

	typedef _jobject*       jobject     ;	typedef _jclass*        jclass       ;	typedef _jstring*       jstring      ;
	typedef _jarray*        jarray      ;	typedef _jobjectArray*  jobjectArray ;	typedef _jbooleanArray* jbooleanArray;
	typedef _jbyteArray*    jbyteArray  ;	typedef _jcharArray*    jcharArray   ;	typedef _jshortArray*   jshortArray  ;
	typedef _jintArray*     jintArray   ;	typedef _jlongArray*    jlongArray   ;	typedef _jfloatArray*   jfloatArray  ;
	typedef _jdoubleArray*  jdoubleArray;	typedef _jthrowable*    jthrowable   ;	typedef _jobject*       jweak        ;

	struct _jfieldID ; typedef struct _jfieldID* jfieldID;
	struct _jmethodID; typedef struct _jmethodID* jmethodID;
	struct JNIInvokeInterface;
	typedef union jvalue { jboolean z; jbyte b; jchar c; jshort s; jint i; jlong j; jfloat f; jdouble d; jobject l; } jvalue;

	typedef enum jobjectRefType { JNIInvalidRefType = 0, JNILocalRefType = 1, JNIGlobalRefType = 2, JNIWeakGlobalRefType = 3 } jobjectRefType;
	typedef struct { const char* name; const char* signature; void* fnPtr; } JNINativeMethod;
	struct _JNIEnv; struct _JavaVM;
	typedef const struct JNINativeInterface* C_JNIEnv;
	typedef _JNIEnv JNIEnv;
	typedef _JavaVM JavaVM;
	struct  JNINativeInterface;
#endif

#if defined(ANDROID) || defined(__ANDROID__)
  #include <jni.h>
  #include <android/log.h>
  #define  LOG_TAG    "AOS"
  #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
  #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO , LOG_TAG, __VA_ARGS__)
  #define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN , LOG_TAG, __VA_ARGS__)
  #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
  #define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#else
  #define LOGD(...){ printf("[D]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
  #define LOGI(...){ printf("[I]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
  #define LOGW(...){ printf("[W]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
  #define LOGE(...){ printf("[E]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
  #define LOGF(...){ printf("[F]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
  #define LOG (...){ printf("[V]:: %s %4d  %s :: ", __FILE__, __LINE__, __func__); printf(__VA_ARGS__); printf("\n");}
#endif

enum
{	// application defined message
	NSDK_EFAIL                              = -1,
	NSDK_OK                                 =  0,				// NSDK_MSG_SUCCESS,
	NSDK_FALSE                              =  0,
	NSDK_TRUE                               =  1,
	NSDK_REQUEST                            =  0x7FFF0001,		// while query ...
	NSDK_EMPTY                              =  0x000000FF,		// empty
	NSDK_ERR_PROC                           =  0xEE000000,		// error process
};

extern const char* AppResourcePath(const char* fileName);
extern const char *GetBundleFileName ( const char *fileName );
extern void WinErrorMessageBox(const char* functionLastCalled, int errorCode);
extern bool checkGLError(const char* functionLastCalled);
extern int LoadTGA(int* oW, int* oH, int* oD, unsigned char** oB, const char* _src_buf, const size_t size);
extern int LoadRAW(int* oW, int* oH, int* oD, unsigned char** oB, const char *sFile);

class FileData
{
public:
	FileData(const char* fileName);
	~FileData();
	int   size() { return m_size; }
	char* data() { return m_data; }
protected:
	int   m_size;
	char* m_data;
};

#endif

