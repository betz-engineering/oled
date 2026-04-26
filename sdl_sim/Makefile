vpath %.c ../firmware
vpath %.c ../firmware/lib
vpath %.c ../font_lib/lib/font_lib

LDLIBS = -lm $(shell pkg-config --libs sdl2)

CFLAGS += -I../firmware -I../firmware/lib -I../font_lib/lib/font_lib -g $(shell pkg-config --cflags sdl2)
CFLAGS += -Wall -DSDL_SIM -g
CFLAGS += -DFB_WIDTH=256 -DFB_HEIGHT=64 -DFB_BPP=4 -DDEBUG=1

OBJS = frame_buffer.o font.o graphics.o print.o demo.o gui.o

all: test

test: $(OBJS)

clean:
	rm -rf test $(OBJS)
