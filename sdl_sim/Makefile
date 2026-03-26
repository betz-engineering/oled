vpath %.c ../firmware
vpath %.c ../firmware/lib

LDLIBS = -lm $(shell pkg-config --libs sdl2)

CFLAGS += -I../firmware -I../firmware/lib -g $(shell pkg-config --cflags sdl2)
CFLAGS += -Wall -DSDL_SIM -g

OBJS = frame_buffer.o lv_font.o print.o demo.o gui.o
OBJS += lv_font_roboto_12.o lv_font_roboto_mono_17.o lv_font_fa.o

all: test

test: $(OBJS)

clean:
	rm -rf test $(OBJS)
