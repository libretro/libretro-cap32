TARGET_NAME := cap32

DEBUG   = 0
LOG_PERFORMANCE = 0
HAVE_COMPAT = 0
HAVE_LIBCO = 0

SOURCES_C   :=
SOURCES_CXX :=
LIBS    :=

ifeq ($(platform),)
	platform = unix
	ifeq ($(shell uname -a),)
		platform = win
	else ifneq ($(findstring MINGW,$(shell uname -a)),)
		platform = win
	else ifneq ($(findstring Darwin,$(shell uname -a)),)
		platform = osx
	else ifneq ($(findstring win,$(shell uname -a)),)
		platform = win
	endif
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
	EXE_EXT = .exe
	system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	system_platform = osx
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	system_platform = win
endif

CC_AS ?= $(CC)
LIBM  := -lm
# Unix
ifneq (,$(findstring unix,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined

else ifneq (,$(findstring linux-portable,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC -nostdlib
	LIBM :=
	SHARED := -shared -Wl,-version-script=link.T
# android arm
else ifneq (,$(findstring android,$(platform)))
   	TARGET := $(TARGET_NAME)_libretro.so
   	fpic = -fPIC
   	SHARED := -lstdc++ -llog -lz -shared -Wl,--version-script=link.T -Wl,--no-undefined

   	CC = arm-linux-androideabi-gcc
   	CXX = arm-linux-androideabi-g++
# Raspberry Pi
else ifneq (,$(findstring rpi,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	LDFLAGS += -shared -Wl,--version-script=libretro/link.T
	fpic = -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined

# OS X
else ifeq ($(platform), osx)
	TARGET := $(TARGET_NAME)_libretro.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
	OSXVER = `sw_vers -productVersion | cut -d. -f 2`
	OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
	ifeq ($(OSX_LT_MAVERICKS),"YES")
		fpic += -mmacosx-version-min=10.5
	endif

# iOS
else ifneq (,$(findstring ios,$(platform)))

	TARGET := $(TARGET_NAME)_libretro_ios.dylib
	fpic := -fPIC
	SHARED := -dynamiclib

	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
	endif

	CC = cc -arch armv7 -isysroot $(IOSSDK)
	CC_AS = perl ./tools/gas-preprocessor.pl $(CC)
	CXX = c++ -arch armv7 -isysroot $(IOSSDK)
ifeq ($(platform),ios9)
	CC += -miphoneos-version-min=8.0
	CXX += -miphoneos-version-min=8.0
	CC_AS += -miphoneos-version-min=8.0
	PLATFORM_DEFINES := -miphoneos-version-min=8.0
else
	CC += -miphoneos-version-min=5.0
	CXX += -miphoneos-version-min=5.0
	CC_AS += -miphoneos-version-min=5.0
	PLATFORM_DEFINES := -miphoneos-version-min=5.0
endif

# tvOS
else ifeq ($(platform), tvos-arm64)
	TARGET := $(TARGET_NAME)_libretro_tvos.dylib
	fpic := -fPIC
	SHARED := -dynamiclib

	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
	endif

# Theos
else ifeq ($(platform), theos_ios)
	DEPLOYMENT_IOSVERSION = 5.0
	TARGET = iphone:latest:$(DEPLOYMENT_IOSVERSION)
	ARCHS = armv7 armv7s
	TARGET_IPHONEOS_DEPLOYMENT_VERSION=$(DEPLOYMENT_IOSVERSION)
	THEOS_BUILD_DIR := objs
	include $(THEOS)/makefiles/common.mk

	LIBRARY_NAME = $(TARGET_NAME)_libretro_ios

# QNX
else ifeq ($(platform), qnx)
	TARGET := $(TARGET_NAME)_libretro_$(platform).so
	fpic := -fPIC
	SHARED := -lcpp -shared -Wl,-version-script=link.T
	CC = qcc -Vgcc_ntoarmv7le
	CC_AS = qcc -Vgcc_ntoarmv7le
	CXX = QCC -Vgcc_ntoarmv7le_cpp
	AR = QCC -Vgcc_ntoarmv7le
	PLATFORM_DEFINES := -fexceptions -marm -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=softfp
	CFLAGS += -std=c99 -D_POSIX_C_SOURCE

# PS3
else ifeq ($(platform), ps3)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
	CC_AS = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-gcc.exe
	CXX = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-g++.exe
	AR = $(CELL_SDK)/host-win32/ppu/bin/ppu-lv2-ar.exe
	PLATFORM_DEFINES := -D__CELLOS_LV2__ -Iutils/zlib
	STATIC_LINKING = 1
	HAVE_COMPAT = 1

# sncps3
else ifeq ($(platform), sncps3)
	TARGET := $(TARGET_NAME)_libretro_ps3.a
	CC = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
	CC_AS = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
	CXX = $(CELL_SDK)/host-win32/sn/bin/ps3ppusnc.exe
	AR = $(CELL_SDK)/host-win32/sn/bin/ps3snarl.exe
	PLATFORM_DEFINES := -D__CELLOS_LV2__
	STATIC_LINKING = 1
	HAVE_COMPAT = 1

# Lightweight PS3 Homebrew SDK
else ifeq ($(platform), psl1ght)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(PS3DEV)/ppu/bin/ppu-gcc$(EXE_EXT)
	CC_AS = $(PS3DEV)/ppu/bin/ppu-gcc$(EXE_EXT)
	CXX = $(PS3DEV)/ppu/bin/ppu-g++$(EXE_EXT)
	AR = $(PS3DEV)/ppu/bin/ppu-ar$(EXE_EXT)
	PLATFORM_DEFINES := -D__CELLOS_LV2__ -D__PSL1GHT__
	STATIC_LINKING = 1
	HAVE_COMPAT = 1

# PSP
else ifeq ($(platform), psp1)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = psp-gcc$(EXE_EXT)
	CC_AS = psp-gcc$(EXE_EXT)
	CXX = psp-g++$(EXE_EXT)
	AR = psp-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DPSP
	CFLAGS += -G0
	CXXFLAGS += -G0
	STATIC_LINKING = 1
	HAVE_COMPAT = 1
	EXTRA_INCLUDES := -I$(shell psp-config --pspsdk-path)/include

# Vita
else ifeq ($(platform), vita)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = arm-vita-eabi-gcc$(EXE_EXT)
	CXX = arm-vita-eabi-g++$(EXE_EXT)
	AR = arm-vita-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DVITA -DNO_UNALIGNED_ACCESS
	STATIC_LINKING = 1

# CTR (3DS)
else ifeq ($(platform), ctr)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITARM)/bin/arm-none-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITARM)/bin/arm-none-eabi-g++$(EXE_EXT)
	AR = $(DEVKITARM)/bin/arm-none-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES := -DARM11 -D_3DS -DNO_UNALIGNED_ACCESS -DM16B -DLOWRES
	PLATFORM_DEFINES += -march=armv6k -mtune=mpcore -mfloat-abi=hard
	PLATFORM_DEFINES += -Wall -mword-relocations
	PLATFORM_DEFINES += -fomit-frame-pointer -ffast-math
	CXXFLAGS += -fno-rtti -fno-exceptions
	STATIC_LINKING = 1

# Xbox 360
else ifeq ($(platform), xenon)
	TARGET := $(TARGET_NAME)_libretro_xenon360.a
	CC = xenon-gcc$(EXE_EXT)
	CC_AS = xenon-gcc$(EXE_EXT)
	CXX = xenon-g++$(EXE_EXT)
	AR = xenon-ar$(EXE_EXT)
	PLATFORM_DEFINES := -D__LIBXENON__
	STATIC_LINKING = 1

# Nintendo Game Cube
else ifeq ($(platform), ngc)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CC_AS = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
	AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES += -DGEKKO -DHW_DOL -mrvl -mcpu=750 -meabi -mhard-float
	STATIC_LINKING = 1
	HAVE_COMPAT = 1

# Nintendo Wii U
else ifeq ($(platform), wiiu)
       TARGET := $(TARGET_NAME)_libretro_$(platform).a
       CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
       CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
       AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
       COMMONFLAGS += -DGEKKO -DWIIU -DHW_RVL -mcpu=750 -meabi -mhard-float
       STATIC_LINKING = 1
       PLATFORM_DEFINES += $(COMMONFLAGS) -Iutils/zlib
       HAVE_COMPAT = 1

# Nintendo Wii
else ifeq ($(platform), wii)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CC_AS = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
	CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
	AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
	PLATFORM_DEFINES += -DGEKKO -DHW_RVL -DLOWRES -mrvl -mcpu=750 -meabi -mhard-float
	STATIC_LINKING = 1
	HAVE_COMPAT = 1

# Nintendo Switch (libnx)
else ifeq ($(platform), libnx)
	include $(DEVKITPRO)/libnx/switch_rules
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CFLAGS += -O3 -fomit-frame-pointer -ffast-math -I$(DEVKITPRO)/libnx/include/ -fPIE -Wl,--allow-multiple-definition
	CFLAGS += -specs=$(DEVKITPRO)/libnx/switch.specs
	CFLAGS += -D__SWITCH__ -DHAVE_LIBNX -DHAVE_GETPWUID=0 -DHAVE_GETCWD=1
	CFLAGS += -march=armv8-a -mtune=cortex-a57 -mtp=soft -ffast-math -mcpu=cortex-a57+crc+fp+simd -ffunction-sections
	CFLAGS += -Ifrontend/switch -ftree-vectorize
	STATIC_LINKING = 1

# ARM
else ifneq (,$(findstring armv,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=link.T
	CC = gcc
	CC_AS = gcc
	CXX = g++
	ifneq (,$(findstring cortexa8,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a8
	else ifneq (,$(findstring cortexa9,$(platform)))
		PLATFORM_DEFINES += -marm -mcpu=cortex-a9
	endif
	PLATFORM_DEFINES += -marm
	ifneq (,$(findstring neon,$(platform)))
		PLATFORM_DEFINES += -mfpu=neon
		HAVE_NEON = 1
	endif
	ifneq (,$(findstring softfloat,$(platform)))
		PLATFORM_DEFINES += -mfloat-abi=softfp
	else ifneq (,$(findstring hardfloat,$(platform)))
		PLATFORM_DEFINES += -mfloat-abi=hard
	endif
	PLATFORM_DEFINES += -DARM

# emscripten
else ifeq ($(platform), emscripten)
	TARGET := $(TARGET_NAME)_libretro_$(platform).bc
	PLATFORM_DEFINES +=-s USE_ZLIB=1
	STATIC_LINKING=1
# cross Windows
else ifeq ($(platform), wincross64)
	TARGET := $(TARGET_NAME)_libretro.dll
	AR = x86_64-w64-mingw32-ar
	CC = x86_64-w64-mingw32-gcc
	CXX = x86_64-w64-mingw32-g++
	SHARED := -shared -Wl,--no-undefined -Wl,--version-script=link.T
	LDFLAGS += -static-libgcc -static-libstdc++
	EXTRA_LDF := -lwinmm -Wl,--export-all-symbols
# Windows
else
	TARGET := $(TARGET_NAME)_libretro.dll
	CC ?= gcc
	CC_AS ?= gcc
	CXX ?= g++
	SHARED := -shared -static-libgcc -static-libstdc++ -Wl,-no-undefined -Wl,-version-script=link.T
	LIBS += -lshlwapi
	HAVE_WIN32_MSX_MANAGER = 1

endif

CORE_DIR  := .
CAP32_DIR := $(CORE_DIR)/cap32

ifeq ($(HAVE_COMPAT), 1)
	PLATFORM_DEFINES += -DHAVE_COMPAT
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
	CXXFLAGS += -O0 -g
else ifeq ($(platform), emscripten)
	CFLAGS += -O2
	CXXFLAGS += -O2
else
	CFLAGS += -O3
	CXXFLAGS += -O3
endif

CXXFLAGS += -fno-exceptions -fno-rtti -DHAVE_STDINT_H

ifeq ($(LOG_PERFORMANCE), 1)
	CFLAGS += -DLOG_PERFORMANCE
	CXXFLAGS += -DLOG_PERFORMANCE
endif

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
    CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

DEFINES := -D__LIBRETRO__ $(PLATFORM_DEFINES) -DINLINE="inline"
DEFINES += -DHAVE_CONFIG_H

CFLAGS   += $(fpic) $(DEFINES)
CFLAGS   += -Wall

CXXFLAGS += $(fpic) $(DEFINES)
CXXFLAGS += -Wall

LDFLAGS += $(LIBM)

ROMS =
SNAPS =

include Makefile.common

HEADERS += $(ROMS:.rom=.h) $(SNAPS:.szx=.h)
OBJS += $(SOURCES_C:.c=.o) $(SOURCES_CXX:.cpp=.o)

INCDIRS := $(EXTRA_INCLUDES) $(INCFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCDIRS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

%.o: %.S
	$(CC_AS) -c -o $@ $< $(CFLAGS) $(INCDIRS)

%.h: %.rom
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.szx
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

ifeq ($(platform), theos_ios)
COMMON_FLAGS := -DIOS $(COMMON_DEFINES) $(INCFLAGS) -I$(THEOS_INCLUDE_PATH) -Wno-error
$(LIBRARY_NAME)_CFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CXXFLAGS += $(CXXFLAGS) $(COMMON_FLAGS)
${LIBRARY_NAME}_FILES = $(SOURCES_CXX) $(SOURCES_C)
${LIBRARY_NAME}_LIBRARIES = z
include $(THEOS_MAKE_PATH)/library.mk
else
all: $(TARGET)

$(TARGET): $(HEADERS) $(OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJS)
else
	$(CC) -o $@ $(SHARED) $(OBJS) $(LDFLAGS) $(LIBS)
endif

clean-objs:
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(HEADERS)
	rm -f $(TARGET)

.PHONY: $(TARGET) clean clean-objs
endif
