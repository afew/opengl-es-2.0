
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_MODULE := gamelogic
LOCAL_MODULE_FILENAME := libgamelogic

SRC_PATH  := $(LOCAL_PATH)
SRC_LIST  := $(wildcard $(SRC_PATH)/*.cpp)

EXC_LIST  :=
SRC_LIST  := $(SRC_LIST:$(LOCAL_PATH)/%=%)
SRC_LIST  := $(filter-out $(EXC_LIST), $(SRC_LIST))


LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include

LOCAL_SRC_FILES  := $(SRC_LIST)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
