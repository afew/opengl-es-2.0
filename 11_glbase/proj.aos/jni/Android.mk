LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#ASSET_FOLDER = $(LOCAL_PATH)/../assets
#ifeq ($(OS),Windows_NT)
#    $(shell mkdir \a $(ASSET_FOLDER))
#    $(shell XCOPY /E /H  $(LOCAL_PATH)\..\..\media $(ASSET_FOLDER)\media)
#else
#    $(shell mkdir -p $(ASSET_FOLDER))
#endif


LOCAL_MODULE := g-pack
LOCAL_MODULE_FILENAME := libg-pack

LOCAL_SRC_FILES  := app_main.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../classes

LOCAL_WHOLE_STATIC_LIBRARIES := gamelogic
LOCAL_CFLAGS    := -w

LOCAL_LDLIBS := -L$(LOCAL_PATH)/../../../lib

LOCAL_LDLIBS += -lglcmath30
LOCAL_LDLIBS += -llog -lGLESv2 -landroid

include $(BUILD_SHARED_LIBRARY)


$(call import-add-path,$(LOCAL_PATH)/../..)
$(call import-module,classes)