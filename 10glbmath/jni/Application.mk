
NDK_TOOLCHAIN_VERSION := clang
APP_PLATFORM := android-15
APP_ABI      := armeabi
APP_STL      := gnustl_static
APP_CPPFLAGS += -marm -march=armv5te -mtune=xscale -msoft-float
APP_OPTIM    := release

APP_SHORT_COMMANDS := true
APP_CPPFLAGS += -fpermissive

