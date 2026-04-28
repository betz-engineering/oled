#include "demo.h"
#include "font.h"
#include "font_awesome_5_free.h"
#include "frame_buffer.h"
#include "graphics.h"
#include "gui.h"
#include "lib/gui.h"
#include "lib/ui_board.h"
#include "lv_symbols.h"
#include "print.h"
#include "roboto.h"
#include "roboto_mono.h"
#include "ssd1322.h"
#include "ui_board.h"
#include <stdio.h>
#include <stdlib.h>

void demo(void) {
    static bool is_inverted = false, is_left_led = false, is_ui_board_1u = false;
    static unsigned frm = 0, leda = 0, ledb = 0, ios_d = 1;
    static int ticks_d = 0;
    static t_label l_ticks, l_io, l_leda, l_ledb, l_symbol;

    if (frm == 0) {
        printf("Clearing the screen and re-initializing\n");
        fill(0);
        lv_init_label(&l_leda, 5, 24, &f_roboto_mono, "0", A_LEFT, true);
        lv_init_label(&l_ledb, 20, 24, &f_roboto_mono, "0", A_LEFT, true);
        lv_triple(&l_ticks, 40, 16, &f_roboto_mono, "Enc:", "-1000", "ticks");
        lv_triple(&l_io, 40, 34, &f_roboto, "IO:", "00000000", "");
        lv_init_label(&l_symbol, 230, 35, &f_font_awesome_5_free, "  ", A_CENTER, true);
    }

    unsigned events = get_event_flags();   // returns state of encoder and back button
    int ticks = get_encoder_ticks(false);  // returns absolute encoder position
    int diff = ticks - ticks_d;

    static unsigned events_d = 0;
    if (events_d != events) {
        if (events != 0)
            printf("events: %04x  ticks: %d\n", events, ticks);
        events_d = events;
    }

    if (diff != 0 || frm == 0) {
        lv_update_label_dp(&l_ticks, ticks, 3, 0);

        // Symbols show-case
        lv_update_label(&l_symbol, all_symbols[abs(ticks) % N_SYMBOLS]);

        // Greyscale bar at the bottom
        set_draw_mode(DRAW_SET);
        for (int x = 0; x < FB_WIDTH; x += 8)
            fill_rectangle(x, 55, x + 7, 63, (x / 8 - ticks) << 4);

        if (is_left_led) {
            leda = (leda + diff) & 0x7;
        } else {
            ledb = (ledb + diff) & 0x7;
        }
    }

    unsigned ios = get_gpios();
    if (ios != ios_d) {
        lv_update_label_bin(&l_io, ios, 8);
        ios_d = ios;
    }

    if (events & EV_ENC_L) {
        is_ui_board_1u = !is_ui_board_1u;
        ui_init(is_ui_board_1u);
        frm = 0;  // force a full display refresh
        return;
    }

    if (events & EV_BACK_L) {
        is_inverted = !is_inverted;
        set_inverted(is_inverted);
    }

    bool update = false;
    if (events & EV_ENC_S) {
        is_left_led = !is_left_led;
        update = true;
    }
    if (events & EV_BACK_S) {
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
