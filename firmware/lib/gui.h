#ifndef GUI_H
#define GUI_H

#include "font.h"

//-----------------------------------------------
// Dirty GUI stuff
//-----------------------------------------------
// this is all for legacy compatibility with Marble-MMC.
// New developments should rather use font_lib directly.

// Horizontal alignment
typedef enum {
       LV_LEFT = H_LEFT,
       LV_CENTER = H_MIDDLE,
       LV_RIGHT = H_RIGHT,
       LV_RIGHT_REF_LEFT = 0xF0
} t_align;

typedef struct {
    // text origin
    int x;
    int y;
    fnt_align_t align;
    // clip window
    bbox_t bb;
    // font
    const font_header_t *fnt;
} t_label;

// Initialize a `label`, which has a fixed bounding box (the area which is
// erased on redraw) and text alignment (left, center, right)
// use lv_update_label to change the label content
// x, y:    position of anchor point, which is on the top
//          left / middle / right, depending on the chosen alignment
// fnt:     the font header file to use
// init:    a string to determine width of the bounding box
// a:       text alignment __AND__ anchor point position
// draw:    if true, also draw the init string to the framebuffer
void lv_init_label(
    t_label *lbl, int x, int y, const font_header_t *fnt, const char *init, t_align a, bool draw);

// Update the text in a label
void lv_update_label(t_label *lbl, const char *buf);

// call it like printf
void lv_update_labelf(t_label *lbl, const char *format, ...);

// Update the text in a label with a fixed point fractional number
void lv_update_label_fix(t_label *lbl, int32_t val, const uint8_t nFract, uint8_t nDigits);

// Update the text in a label with a n-digit decimal number with decimal point fixed at dp
void lv_update_label_dp(t_label *lbl, int32_t val, const uint8_t n, const uint8_t dp);

// Update the text in a label with a n-digit number in hex format
void lv_update_label_hex(t_label *lbl, int32_t val, uint8_t nDigits);

// Update the text in a label with a n-digit number in binary format
void lv_update_label_bin(t_label *lbl, uint32_t val, uint8_t nDigits);

// Draw a border around the element
void lv_border(t_label *lbl);

// Convenience shortcut to initialize 3 labels:
// <a: static label><b: dynamic number><c: static unit>
// use `lv_update_label(nmb, ...)` to update the dynamic number in the middle
// x, y:    position of anchor point on the top left
void lv_triple(
    t_label *nmb, int x, int y, const font_header_t *fnt, const char *a, const char *b, const char *c);


// Compatibility layer between the old graphics functions and font_lib

#define LV_BPP FB_BPP
#define DISPLAY_WIDTH  FB_WIDTH
#define DISPLAY_HEIGHT  FB_HEIGHT

// Draw a rectangle from (x1, y1) to (x2, y2)
void fillRect(int x1, int y1, int x2, int y2, uint8_t shade);

void rect(int x0, int x1, int y0, int y1, uint8_t shade);
void emptyRoundedRect(int x1, int y1, int x2, int y2, int radius, int thickness);

void invertRect(int x1, int y1, int x2, int y2);
void invertRoundedRect(int x1, int y1, int x2, int y2, int radius);

#endif
