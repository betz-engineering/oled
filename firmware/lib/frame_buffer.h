#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H
#include <stdint.h>

#define LV_BPP 4
#define DISPLAY_WIDTH 256
#define DISPLAY_HEIGHT 64

extern uint8_t g_frameBuff[DISPLAY_HEIGHT * DISPLAY_WIDTH / 2];

// Keep track of the rectangular area of the display which actually changed
// To do partial display writes
void update_window(int x0, int y0, int x1, int y1);

// SET / GET a single pixel to a specific shade (0 - 15) in the framebuffer
void setPixel(unsigned x, unsigned y, uint8_t shade);
void addPixel(unsigned x, unsigned y, uint8_t shade);
uint8_t getPixel(unsigned x, unsigned y);

// Set whole screen to fixed shade
void fill(uint8_t shade);

// Draw a rectangle from (x0, y0) to (x1, y1)
void fillRect(int x0, int y0, int x1, int y1, uint8_t shade);
void rect(int x0, int y0, int x1, int y1, uint8_t shade);

// Draw an anti-aliased line from (x0, y0) to (x1, y1)
void drawLine(int x0, int y0, int x1, int y1);

// send the complete framebuffer to the display
void send_fb(void);

// send only the modified window of the framebuffer to the display
void send_partial_fb(void);
#endif
