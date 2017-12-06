LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE          := mediarecorder
LOCAL_MODULE_FILENAME := libmediarecorder

ifdef _PRIMARY_WORK_
  LOCAL_CFLAGS  += -D_PRIMARY_WORK_
else
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx/cocos
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cocos2dx/include
endif

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_SRC_FILES := MediaRecorder.cpp SimpleAVRecorderAOS.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_STATIC_LIBRARY)
