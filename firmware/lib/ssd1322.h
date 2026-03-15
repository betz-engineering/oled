#pragma once
#include <stdbool.h>
#include <stdint.h>

void init_ssd1322(void);

// set OLED brightness (0 = off, 1 - 16 = on)
void set_brightness(uint8_t val);

// invert the display
void set_inverted(bool val);

// send the complete framebuffer to the display.
// p needs to point to a buffer of size 8192 bytes
void write_vram(uint8_t *p);

// send a certain rectangular window of the framebuffer to the display
// x1, y1, x2, y2: the rectangle to update in [pixels]
// x1, y1, x2 and y2 are all inclusive!
// note that ssd1322 works with columns of 4 pixels horizontally
// so the lower 2 bits of x1 and x2 will be truncated
// data in 4 bits / pixel, 2 pixels / byte
void send_window_4(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint8_t *data);
