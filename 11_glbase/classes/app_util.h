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
#if defined(_ANDROID_) || defined(__ANDROID__)
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

