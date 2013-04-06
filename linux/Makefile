platform = unix

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

ifeq ($(platform), unix)
   CC = gcc
   TARGET := libretro-cap32.so
   fpic := -fPIC
   SHARED := -shared -Wl,--version-script=../libretro/link.T -Wl,--no-undefined -fPIC
else ifeq ($(platform), osx)
   TARGET := libretro.dylib
   fpic := -fPIC
   SHARED := -dynamiclib
else
   CC = gcc
   TARGET := retro-euae.dll
   SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=../libretro/link.T -Wl,--no-undefined
endif

ifeq ($(DEBUG), 1)
   CFLAGS += -O0 -g
else
   CFLAGS += -O3
endif

EMU = ../cap32/capold-4.2.0

CORE_SRCS :=$(EMU)/z80.o $(EMU)/tape.o $(EMU)/psg.o $(EMU)/fdc.o $(EMU)/crtc.o $(EMU)/cap32.o 

BUILD_APP =  $(CORE_SRCS) 

HINCLUDES := -I./$(EMU) -I../libretro 

OBJECTS := $(BUILD_APP) ../libretro/libretro-cap32.o ../libretro/cap32-mapper.o ../libretro/vkbd.o \
	../libretro/graph.o ../libretro/diskutils.o ../libretro/fontmsx.o  

DEFINES +=  -DLSB_FIRST -DALIGN_DWORD 
CFLAGS += $(DEFINES) -DRETRO=1 -std=gnu99  -O3 -finline-functions -funroll-loops  -funsigned-char  \
	-Wno-strict-prototypes -ffast-math -fomit-frame-pointer -fno-strength-reduce  -fno-builtin -finline-functions -s -fPIC

CXXFLAGS  +=	$(CFLAGS) -std=gnu++0x
CPPFLAGS += $(CFLAGS)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(fpic) $(SHARED) $(INCLUDES) -o $@ $(OBJECTS) -lm -lz 
	
%.o: %.c
	$(CC) $(CFLAGS) $(HINCLUDES) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(HINCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean

