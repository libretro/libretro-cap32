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
	arch = intel
	ifeq ($(shell uname -p),powerpc)
		arch = ppc
	endif
	ifeq ($(shell uname -p),arm)
		arch = arm
	endif
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
	CFLAGS += -Wall -mword-relocations -fomit-frame-pointer -ffast-math
	CFLAGS += -falign-functions=1 -falign-jumps=1 -falign-loops=1

	TARGET := $(TARGET_NAME)_libretro.so
	fpic = -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined

	ifneq (,$(findstring rpi1,$(platform)))
		CFLAGS := -DFRONTEND_SUPPORTS_RGB565 -DLOWRES -DINLINE="inline" -DM16B
#		Raspberry Pi 1 - Model B (original)
		ifneq (,$(findstring rpi1bo,$(platform)))
			CFLAGS += -funsafe-math-optimizations -fsingle-precision-constant -fexpensive-optimizations
			CFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops
			CFLAGS += -march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp
			LDFLAGS += -march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp
		else
			CFLAGS += -march=armv6j -mfpu=vfp -mfloat-abi=hard -marm
			LDFLAGS += -march=armv6j -mfpu=vfp -mfloat-abi=hard -marm
		endif
	else ifneq (,$(findstring rpi2,$(platform)))
		CFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -marm
		LDFLAGS += -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -marm
	else ifneq (,$(findstring rpi3,$(platform)))
		CFLAGS += -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -marm
		LDFLAGS += -mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -marm
	endif
# evercade armv7
else ifneq (,$(findstring evercade,$(platform)))
	TARGET := $(TARGET_NAME)_libretro.so
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined
	CC = /opt/evercade/usr/bin/arm-linux-gnueabihf-gcc
	CC_AS = /opt/evercade/usr/bin/arm-linux-gnueabihf-as
	AR = /opt/evercade/usr/bin/arm-linux-gnueabihf-ar
	CFLAGS := -DFRONTEND_SUPPORTS_RGB565 -DLOWRES -DINLINE="inline" -DM16B -DEVERCADE
	CFLAGS += -falign-functions=1 -falign-jumps=1 -falign-loops=1
	CFLAGS += -fomit-frame-pointer -ffast-math	
	CFLAGS += -funsafe-math-optimizations -fsingle-precision-constant -fexpensive-optimizations
	CFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops

# OS X
else ifeq ($(platform), osx)
	TARGET := $(TARGET_NAME)_libretro.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
        MINVERSION :=
	OSXVER = `sw_vers -productVersion | cut -d. -f 2`
	OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
	ifeq ($(OSX_LT_MAVERICKS),"YES")
		MINVERSION = -mmacosx-version-min=10.5
	endif
	fpic += $(MINVERSION)

   ifeq ($(CROSS_COMPILE),1)
		TARGET_RULE   = -target $(LIBRETRO_APPLE_PLATFORM) -isysroot $(LIBRETRO_APPLE_ISYSROOT)
		CFLAGS   += $(TARGET_RULE)
		CPPFLAGS += $(TARGET_RULE)
		CXXFLAGS += $(TARGET_RULE)
		LDFLAGS  += $(TARGET_RULE)
   endif

	CFLAGS  += $(ARCHFLAGS)
	CXXFLAGS  += $(ARCHFLAGS)
	LDFLAGS += $(ARCHFLAGS)

# iOS
else ifneq (,$(findstring ios,$(platform)))

	TARGET := $(TARGET_NAME)_libretro_ios.dylib
	fpic := -fPIC
	SHARED := -dynamiclib
        MINVERSION :=

	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
	endif

ifeq ($(platform),ios-arm64)
	CC    = cc -arch arm64 -isysroot $(IOSSDK)
	CC_AS = perl ./tools/gas-preprocessor.pl $(CC)
	CXX   = c++ -arch arm64 -isysroot $(IOSSDK)
else
	CC    = cc -arch armv7 -isysroot $(IOSSDK)
	CC_AS = perl ./tools/gas-preprocessor.pl $(CC)
	CXX   = c++ -arch armv7 -isysroot $(IOSSDK)
endif
ifeq ($(platform),$(filter $(platform),ios9 ios-arm64))
	MINVERSION = -miphoneos-version-min=8.0
else
	MINVERSION = -miphoneos-version-min=5.0
endif
	PLATFORM_DEFINES := $(MINVERSION)

# tvOS
else ifeq ($(platform), tvos-arm64)
	TARGET := $(TARGET_NAME)_libretro_tvos.dylib
	fpic := -fPIC
	SHARED := -dynamiclib

	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
	endif

	CC    = cc -arch arm64 -isysroot $(IOSSDK)
	CC_AS = perl ./tools/gas-preprocessor.pl $(CC)
	CXX   = c++ -arch arm64 -isysroot $(IOSSDK)

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

# Lightweight PS3 Homebrew SDK
else ifneq (,$(filter $(platform), ps3 psl1ght))
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	CC = $(PS3DEV)/ppu/bin/ppu-$(COMMONLV)gcc$(EXE_EXT)
	CC_AS = $(PS3DEV)/ppu/bin/ppu-$(COMMONLV)gcc$(EXE_EXT)
    CXX = $(PS3DEV)/ppu/bin/ppu-$(COMMONLV)g++$(EXE_EXT)
    AR = $(PS3DEV)/ppu/bin/ppu-$(COMMONLV)ar$(EXE_EXT)
	PLATFORM_DEFINES := -D__PS3__
	STATIC_LINKING = 1
	HAVE_COMPAT = 1
    ifeq ($(platform), psl1ght)
        PLATFORM_DEFINES += -D__PSL1GHT__
    endif

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

#RETROFW
else ifeq ($(platform), retrofw)
	TARGET := $(TARGET_NAME)_libretro.so
	CC = /opt/retrofw-toolchain/usr/bin/mipsel-linux-gcc
	CC_AS = /opt/retrofw-toolchain/usr/bin/mipsel-linux-as
	CXX = /opt/retrofw-toolchain/usr/bin/mipsel-linux-g++
	AR = /opt/retrofw-toolchain/usr/bin/mipsel-linux-ar
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined
	CFLAGS := -DFRONTEND_SUPPORTS_RGB565  -DLOWRES -DINLINE="inline" -DM16B
	CFLAGS += -ffast-math -march=mips32 -mtune=mips32 -mhard-float
	CFLAGS += -falign-functions=1 -falign-jumps=1 -falign-loops=1
	CFLAGS += -fomit-frame-pointer -ffast-math	
	CFLAGS += -funsafe-math-optimizations -fsingle-precision-constant -fexpensive-optimizations
	CFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops
	
#MIYOO
else ifeq ($(platform), miyoo)
	TARGET := $(TARGET_NAME)_libretro.so
   	CC = /opt/miyoo/usr/bin/arm-linux-gcc
   	CC_AS = /opt/miyoo/usr/bin/arm-linux-as
   	CXX = /opt/miyoo/usr/bin/arm-linux-g++
   	AR = /opt/miyoo/usr/bin/arm-linux-ar
	fpic := -fPIC
	SHARED := -shared -Wl,-version-script=link.T -Wl,-no-undefined
	CFLAGS := -DFRONTEND_SUPPORTS_RGB565  -DLOWRES -DINLINE="inline" -DM16B
	CFLAGS += -ffast-math -march=armv5te -mtune=arm926ej-s
	CFLAGS += -falign-functions=1 -falign-jumps=1 -falign-loops=1
	CFLAGS += -fomit-frame-pointer -ffast-math	
	CFLAGS += -funsafe-math-optimizations -fsingle-precision-constant -fexpensive-optimizations
	CFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-unroll-loops

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
else ifeq ($(platform), retrofw)
	CFLAGS += -Ofast
	CXXFLAGS += -Ofast
else
	CFLAGS += -O3
	CXXFLAGS += -O3
endif

CXXFLAGS += -fno-exceptions -fno-rtti -DHAVE_STDINT_H

ifeq ($(LOG_PERFORMANCE), 1)
	CFLAGS += -DLOG_PERFORMANCE
	CXXFLAGS += -DLOG_PERFORMANCE
endif

ifeq ($(DEBUG_TEST), 1)
CFLAGS += -D_UNITTEST_DEBUG
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

# List of dependencies defined from list of object files
DEPS := $(OBJS:.o=.d)

INCDIRS := $(EXTRA_INCLUDES) $(INCFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INCDIRS)

%.o: %.c
	@echo $@
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCDIRS)

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
	@$(CC) -o $@ $(SHARED) $(OBJS) $(LDFLAGS) $(LIBS)
endif

clean-objs:
	rm -f $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(HEADERS)
	rm -f $(TARGET)
	rm -f unit-tests/*.o unit-tests/autorun unit-tests/test-db

rmp:
	$(CORE_DIR)/contrib/remap2db.py $(CORE_DIR)/contrib/remaps $(CORE_DIR)/libretro-common/include/libretro.h > $(CORE_DIR)/libretro/db/entries.h
	rm -f $(CORE_DIR)/libretro/db/*.o

# unit test lib
$(CORE_DIR)/unit-tests/cmocka.o: 
	$(CC) -c -o $(CORE_DIR)/unit-tests/cmocka.o $(CORE_DIR)/cmocka/src/cmocka.c $(CFLAGS) -I$(CORE_DIR)/cmocka/include $(INCDIRS)

# to get more info: make clean && make unit-test DEBUG_TEST=1
unit-test: $(CORE_DIR)/unit-tests/cmocka.o $(OBJS)
	@$(CC) -c -o $(CORE_DIR)/unit-tests/test-utils.o $(CORE_DIR)/unit-tests/test-utils.c $(CFLAGS) -Wno-implicit-function-declaration $(INCDIRS) -I$(CORE_DIR)/cmocka/include
	@$(CC) -c -o $(CORE_DIR)/unit-tests/md5.o $(CORE_DIR)/libretro-common/utils/md5.c $(CFLAGS) -Wno-implicit-function-declaration $(INCDIRS) -I$(CORE_DIR)/libretro-common/include/utils
	@$(CC) -o $(CORE_DIR)/unit-tests/autorun $(CORE_DIR)/unit-tests/autorun.c $(OBJS) $(CORE_DIR)/unit-tests/cmocka.o $(CORE_DIR)/unit-tests/md5.o $(CORE_DIR)/unit-tests/test-utils.o $(LDFLAGS) $(TEST_FLAGS) $(CFLAGS) -Wno-unused-function -I$(CORE_DIR)/cmocka/include $(INCDIRS)
	$(CORE_DIR)/unit-tests/autorun

unit-test-db: $(CORE_DIR)/unit-tests/cmocka.o $(OBJS)
	@$(CC) -c -o $(CORE_DIR)/unit-tests/test-utils.o $(CORE_DIR)/unit-tests/test-utils.c $(CFLAGS) -Wno-implicit-function-declaration $(INCDIRS) -I$(CORE_DIR)/cmocka/include
	@$(CC) -c -o $(CORE_DIR)/unit-tests/md5.o $(CORE_DIR)/libretro-common/utils/md5.c $(CFLAGS) -Wno-implicit-function-declaration $(INCDIRS) -I$(CORE_DIR)/libretro-common/include/utils
	@$(CC) -o $(CORE_DIR)/unit-tests/test-db $(CORE_DIR)/unit-tests/test-db.c $(OBJS) $(CORE_DIR)/unit-tests/cmocka.o $(CORE_DIR)/unit-tests/test-utils.o $(CORE_DIR)/unit-tests/md5.o $(LDFLAGS) $(TEST_FLAGS) $(CFLAGS) -Wno-unused-function -I$(CORE_DIR)/cmocka/include $(INCDIRS)
	@echo "Now run: $(CORE_DIR)/unit-tests/test-db <full-path-to clean-cpc-db roms>"

.PHONY: $(TARGET) clean clean-objs
endif
