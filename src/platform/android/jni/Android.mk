LOCAL_PATH := $(call my-dir)

##bullet
#include $(CLEAR_VARS)
#LOCAL_MODULE := bullet
#LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libBulletSoftBody.a
#LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libBulletDynamics.a
#LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libBulletCollision.a
#LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libLinearMath.a
#LOCAL_EXPORT_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/bullet
#include $(PREBUILT_STATIC_LIBRARY)

#assimp
include $(CLEAR_VARS)
LOCAL_MODULE := assimp
LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libassimp.a
LOCAL_EXPORT_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/assimp
include $(PREBUILT_STATIC_LIBRARY)

#libpng
include $(CLEAR_VARS)
LOCAL_MODULE := libpng
LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libpng.a
LOCAL_EXPORT_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/
include $(PREBUILT_STATIC_LIBRARY)

##libz
#include $(CLEAR_VARS)
#LOCAL_MODULE := libz
#LOCAL_SRC_FILES := $(ANDROID_TOOLCHAIN)/lib/libz.a
#LOCAL_EXPORT_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../3rdparty/eigen3/
#LOCAL_C_INCLUDES := /home/jesse/local/android_toolchain/include/eigen3/
LOCAL_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/
LOCAL_C_INCLUDES += $(ANDROID_TOOLCHAIN)/include/eigen3/
LOCAL_C_INCLUDES += $(ANDROID_SYSROOT)/include/

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_CPPFLAGS  += -std=c++11
LOCAL_SRC_FILES := ../../common/game.cpp jni.cpp assets_file_io.cpp
LOCAL_STATIC_LIBRARIES := assimp libpng
LOCAL_LDLIBS 	:= -lstdc++ -lz -llog -lGLESv2 -lEGL -landroid


include $(BUILD_SHARED_LIBRARY)
