#########1#########2#########3#########4#########5#########6#########7#########8
# global option
#
LIB_NAME  := glcmath30

#########1#########2#########3#########4#########5#########6#########7#########8
# static library compile
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

SRC_PATH  := $(LOCAL_PATH)/..
SRC_LIST  := $(wildcard $(SRC_PATH)/*.cpp)

EXC_LIST  :=
SRC_LIST  := $(SRC_LIST:$(LOCAL_PATH)/%=%)
SRC_LIST  := $(filter-out $(EXC_LIST), $(SRC_LIST))

$(info -------------------------------------------------------------------------------- )
$(info $(SRC_LIST))
$(info file count: $(words $(SRC_LIST)))
$(info -------------------------------------------------------------------------------- )

LOCAL_MODULE          := $(LIB_NAME)
LOCAL_MODULE_FILENAME := lib$(LIB_NAME)

LOCAL_SRC_FILES  := $(SRC_LIST)
LOCAL_C_INCLUDES += $(SRC_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
