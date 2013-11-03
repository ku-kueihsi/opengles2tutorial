LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../3rdparty/eigen3/

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := ../../common/game.cpp jni.cpp assets_file_io.cpp
LOCAL_LDLIBS := -llog -lGLESv2 -lEGL -landroid

include $(BUILD_SHARED_LIBRARY)
