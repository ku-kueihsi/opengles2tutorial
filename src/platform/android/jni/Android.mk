LOCAL_PATH := $(call my-dir)

##eigen3
#include $(CLEAR_VARS)
#LOCAL_MODULE := eigen3
#LOCAL_EXPORT_C_INCLUDES := $(ANDROID_TOOLCHAIN)/include/eigen3
#include $(PREBUILT_STATIC_LIBRARY)

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
LOCAL_MODULE    := game
LOCAL_C_INCLUDES += $(ANDROID_TOOLCHAIN)/include/eigen3/
LOCAL_CFLAGS    := -Wall -Wextra -O2
LOCAL_CFLAGS    += -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -DANDROID -Wa,--noexecstack -std=gnu++0x -mtune=cortex-a9 -march=armv7-a -mhard-float -mfloat-abi=softfp -mfpu=vfpv3-d16 -g -g -gdwarf-2 -marm -O0 -fno-omit-frame-pointer -Wall -W -D_REENTRANT -fPIE -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -DQT_OPENGL_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
LOCAL_CPPFLAGS  += -std=c++11 -O2
LOCAL_CPPFLAGS  += -march=armv7-a -mfloat-abi=softfp -mfpu=vfp -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -DANDROID -Wa,--noexecstack -std=gnu++0x -mtune=cortex-a9 -march=armv7-a -mhard-float -mfloat-abi=softfp -mfpu=vfpv3-d16 -g -g -gdwarf-2 -marm -O0 -fno-omit-frame-pointer -Wall -W -D_REENTRANT -fPIE -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -DQT_OPENGL_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB 
LOCAL_SRC_FILES := ../../common/game.cpp jni.cpp assets_file_io.cpp
LOCAL_STATIC_LIBRARIES := assimp libpng #bullet
LOCAL_LDLIBS 	:= -lz -lstdc++ -llog -lGLESv2 -lEGL -landroid
include $(BUILD_SHARED_LIBRARY)
