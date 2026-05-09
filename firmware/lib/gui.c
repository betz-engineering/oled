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
    lbl->fnt = fnt;
    lbl->x = x;
    lbl->y = y;
    lbl->align = a;
    // special case: x, y refers the the left side but text in label is right-aligned
    // measure the width of the BB while text is left-aligned
    if (a == LV_RIGHT_REF_LEFT)
        lbl->align = H_LEFT;
    fnt_init_from_header(fnt);
    lbl->bb = fnt_measure_text(x, y, init, 64, lbl->align);

    // Then keep the BB but move the anchor point and alignment to the right edge
    if (a == LV_RIGHT_REF_LEFT){
        lbl->x = lbl->bb.right;
        lbl->align = H_RIGHT;
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
    set_draw_mode(DRAW_ADD);
    draw_rectangle_bb(bb_add_spacing(lbl->bb, 2), 0xFF);
}

void lv_update_label(t_label *lbl, const char *buf) {
    set_draw_mode(DRAW_SET);
    fill_rectangle_bb(bb_add_spacing(lbl->bb, 2), 0);

    set_draw_mode(DRAW_ADD);
    fnt_init_from_header(lbl->fnt);
    set_draw_region(lbl->bb.left - 1, lbl->bb.top - 1, lbl->bb.right + 1, lbl->bb.bottom + 1);
    fnt_draw_text(lbl->x, lbl->y, buf, 64, lbl->align);
    set_draw_region_full();
}

void lv_triple(
    t_label *nmb, int x, int y, const font_header_t *fnt, const char *a, const char *b, const char *c) {
    t_label tmp = {0};
    if (a != NULL)
        lv_init_label(&tmp, x, y, fnt, a, H_LEFT, true);
    if (b != NULL)
        lv_init_label(nmb, tmp.bb.right + 4, y, fnt, b, LV_RIGHT_REF_LEFT, false);
    if (c != NULL)
        lv_init_label(&tmp, nmb->bb.right + 4, y, fnt, c, H_LEFT, true);
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

// Draw a rectangle from (x1, y1) to (x2, y2)
void fillRect(int x1, int y1, int x2, int y2, uint8_t shade) {
    set_draw_mode(DRAW_SET);
    fill_rectangle(x1, y1, x2, y2, shade << 8);
}

void rect(int x0, int x1, int y0, int y1, uint8_t shade) {
    set_draw_mode(DRAW_SET);
    draw_rectangle(x0, y0, x1, y1, shade << 8);
}

// Draw empty (outline) rounded rectangle with specified thickness
void emptyRoundedRect(int x1, int y1, int x2, int y2, int radius, int thickness) {
    set_draw_mode(DRAW_ADD);
    draw_rectangle_r(x1, y1, x2, y2, radius, 0xFF);
}

void invertRect(int x1, int y1, int x2, int y2) {
    set_draw_mode(DRAW_INV);
    fill_rectangle(x1, y1, x2, y2, 0xFF);
}

void invertRoundedRect(int x1, int y1, int x2, int y2, int radius) {
    set_draw_mode(DRAW_INV);
    fill_rectangle_r(x1, y1, x2, y2, radius, 0xFF);
}
