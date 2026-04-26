#include "gui.h"
#include "font.h"
#include "frame_buffer.h"
#include "graphics.h"
#include "print.h"
#include <stdarg.h>
#include <stdio.h>

// Initialize a `label`, which has a fixed bounding box and alignment
// x, y = position of anchor point
// fnt = the font header file to use
// init = a string to determine width of the bounding box
// a = text alignment __AND__ anchor point!
// draw = if true, also draw the init string to the framebuffer
// use lv_update_label to change the label content
void lv_init_label(
    t_label *lbl, int x, int y, const font_header_t *fnt, const char *init, t_align a, bool draw) {
    int left = 0, right = 0, top = 0, bottom = 0;
    init_from_header(fnt);
    fnt_get_bb(init, 64, a, &left, &right, &top, &bottom);
    lbl->fnt = fnt;
    lbl->align = a;
    lbl->x = x;
    lbl->y = y;
    lbl->x0 = x + left;
    lbl->x1 = x + right;
    lbl->y0 = y + top;
    lbl->y1 = y + bottom;
    if (a == A_RIGHT_REF_LEFT) {
        // x, y anchor on the left but aligned right
        lbl->x += right;
        lbl->align = A_RIGHT;
    }
    if (draw)
        lv_update_label(lbl, init);
}

// call it like printf
void lv_update_labelf(t_label *lbl, const char *format, ...) {
    char buf[32];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, sizeof(buf), format, argptr);
    va_end(argptr);
    lv_update_label(lbl, buf);
}

void lv_border(t_label *lbl) {
    draw_rectangle(lbl->x0 - 1, lbl->y0 - 1, lbl->x1, lbl->y1, 0xFF);  // show bb
}

void lv_update_label(t_label *lbl, const char *buf) {
    set_draw_mode(DRAW_SET);
    fill_rectangle(lbl->x0, lbl->y0, lbl->x1, lbl->y1, 0x00);
    // draw_rectangle(lbl->x0, lbl->y0, lbl->x1, lbl->y1, 0xFF);  // show bb

    init_from_header(lbl->fnt);
    set_draw_region(lbl->x0, lbl->y0, lbl->x1, lbl->y1);
    set_draw_mode(DRAW_ADD);
    push_str(lbl->x, lbl->y, buf, 64, lbl->align);
}

void lv_triple(
    t_label *nmb, int x, int y, const font_header_t *fnt, const char *a, const char *b, const char *c) {
    t_label tmp;
    lv_init_label(&tmp, x, y, fnt, a, A_LEFT, true);
    lv_init_label(nmb, tmp.x1 + 4, y, fnt, b, A_RIGHT_REF_LEFT, false);
    lv_init_label(&tmp, nmb->x1 + 4, y, fnt, c, A_LEFT, true);
}

void lv_update_label_dp(t_label *lbl, int32_t val, const uint8_t n, const uint8_t dp) {
    char buf[16];
    dec_dp(val, n, dp, buf);
    lv_update_label(lbl, buf);
}

void lv_update_label_fix(t_label *lbl, int32_t val, const uint8_t nFract, uint8_t nDigits) {
    char buf[16];
    dec_fix(val, nFract, nDigits, buf);
    lv_update_label(lbl, buf);
}

void lv_update_label_hex(t_label *lbl, int32_t val, uint8_t nDigits) {
    char buf[16];
    char *p = buf;
    for (int i = (4 * nDigits) - 4; i >= 0; i -= 4)
        *p++ = "0123456789ABCDEF"[(val >> i) % 16];
    *p++ = '\0';
    lv_update_label(lbl, buf);
}

void lv_update_label_bin(t_label *lbl, uint32_t val, uint8_t nDigits) {
    char buf[32];
    char *p = buf;
    for (int i = nDigits - 1; i >= 0; i--)
        if (val & (1 << i))
            *p++ = '1';
        else
            *p++ = '0';
    *p++ = '\0';
    lv_update_label(lbl, buf);
}
