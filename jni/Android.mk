LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_DIR := ../../src

LOCAL_MODULE    := retro-cap32

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -DANDROID_ARM
endif

ifeq ($(TARGET_ARCH),x86)
LOCAL_CFLAGS +=  -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
LOCAL_CFLAGS += -DANDROID_MIPS -D__mips__ -D__MIPSEL__
endif


EMU = ../cap32/capold-4.2.0

CORE_SRCS :=$(EMU)/z80.cpp $(EMU)/tape.cpp $(EMU)/psg.cpp $(EMU)/fdc.cpp $(EMU)/crtc.cpp $(EMU)/cap32.cpp 


BUILD_APP =  $(CORE_SRCS) 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(EMU)   $(LOCAL_PATH)/../libretro  


OBJECTS := $(BUILD_APP) ../libretro/libretro-cap32.c ../libretro/cap32-mapper.c ../libretro/vkbd.c \
	../libretro/graph.c ../libretro/diskutils.c ../libretro/fontmsx.c  


DEFINES +=  -DLSB_FIRST -DALIGN_DWORD 

LOCAL_SRC_FILES    += $(OBJECTS)

LOCAL_CFLAGS +=   $(DEFINES) -DRETRO=1 -DAND -D_ANDROID_ \
		-std=gnu99  -O3 -finline-functions -funroll-loops  -funsigned-char  \
		 -Wno-strict-prototypes -ffast-math -fomit-frame-pointer -fno-strength-reduce  -fno-builtin -finline-functions -s

LOCAL_CXXFLAGS  +=	$(CFLAGS) -std=gnu++0x
LOCAL_CPPFLAGS += $(CFLAGS)

LOCAL_LDLIBS    := -shared -lz -Wl,--version-script=../libretro/link.T 

include $(BUILD_SHARED_LIBRARY)
