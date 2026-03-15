#include "demo.h"
#include "frame_buffer.h"
#include "gui.h"
#include "lib/gui.h"
#include "lv_font.h"
#include "lv_symbols.h"
#include "print.h"
#include "ssd1322.h"
#include "ui_board.h"
#include <stdlib.h>

extern lv_font_t lv_font_roboto_12, lv_font_roboto_mono_17, lv_font_fa;

#define N_SYMBOLS 6
static const char *all_symbols[] = {
    THERMOMETER_FULL, BOLT, PLUG, MICROCHIP, BROADCAST_TOWER, UNLOCK_ALT};

void demo(void) {
    static bool is_inverted = false, is_left_led = true;
    static unsigned frm = 0, leda = 0, ledb = 0, ios_d = 1;
    static int ticks_d = 0;
    static t_label l_ticks, l_io, l_leda, l_ledb, l_symbol;

    int enc = get_encoder_ticks(false);  // returns absolute encoder position
    unsigned btns = get_button_flags();  // returns state of encoder and back button

    if (frm == 0) {
        lv_init_label(&l_leda, 0, 24, &lv_font_roboto_mono_17, "0", LV_LEFT, true);
        lv_init_label(&l_ledb, 15, 24, &lv_font_roboto_mono_17, "0", LV_LEFT, true);
        lv_triple(&l_ticks, 32, 16, &lv_font_roboto_mono_17, "Enc:", "-1000", "ticks");
        lv_triple(&l_io, 32, 34, &lv_font_roboto_12, "IO:", "0000000000000000", "");
        lv_init_label(&l_symbol, 230, 10, &lv_font_fa, BROADCAST_TOWER, LV_CENTER, true);
    }

    int ticks = get_encoder_ticks(false);
    int diff = ticks - ticks_d;

    if (diff != 0) {
        lv_update_label_dp(&l_ticks, ticks, 3, 0);

        // Symbols show-case
        lv_update_label(&l_symbol, all_symbols[abs(ticks) % N_SYMBOLS]);

        // Greyscale bar at the bottom
        for (int x = 0; x < DISPLAY_WIDTH; x += 8)
            fillRect(x, 55, x + 7, 63, (x / 8 + ticks) & 0xF);

        if (is_left_led) {
            leda = (leda + diff) & 0x7;
        } else {
            ledb = (ledb + diff) & 0x7;
        }
    }

    unsigned ios = get_gpios();
    if (ios != ios_d) {
        lv_update_label_bin(&l_io, ios, 16);
        ios_d = ios;
    }

    if (btns & EV_BACK_L) {
        is_inverted = !is_inverted;
        set_inverted(is_inverted);
    }

    bool update = false;
    if (btns & EV_ENC_S) {
        is_left_led = !is_left_led;
        update = true;
    }
    if (btns & EV_ENC_L) {
        leda = 0;
        ledb = 0;
        update = true;
    }
    if (diff != 0 || update || (frm == 0)) {
        set_leda(leda);
        set_ledb(ledb);
        lv_update_label_hex(&l_leda, leda, 1);
        lv_update_label_hex(&l_ledb, ledb, 1);
        lv_border(is_left_led ? &l_leda : &l_ledb);
    }

    frm++;
    ticks_d = ticks;
}
