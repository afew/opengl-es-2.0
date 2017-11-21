LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := g-pack
LOCAL_MODULE_FILENAME := libg-pack

LOCAL_SRC_FILES  := app_main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src

LOCAL_WHOLE_STATIC_LIBRARIES := gamelogic
LOCAL_CFLAGS    := -w
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid

include $(BUILD_SHARED_LIBRARY)


$(call import-add-path,$(LOCAL_PATH)/../../src)
$(call import-module,.)