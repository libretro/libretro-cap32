CC=psp-gcc
AR=psp-ar
RANLIB=psp-ranlib

CFLAGS=-O2 -G0 -Wall -I$(PSPSDK)/include
PSPSDK=$(shell psp-config --pspsdk-path)

OBJS=fs.o
TARGET=libcpccat.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cru $@ $?
	$(RANLIB) $@

clean:
	-@rm -f $(OBJS) $(TARGET)


.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<



