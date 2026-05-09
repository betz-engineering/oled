#ifndef LV_SYMBOLS_H
#define LV_SYMBOLS_H
#define BOLT "\xef\x83\xa7"
#define BROADCAST_TOWER "\xef\x94\x99"
#define CHECK_CIRCLE "\xef\x81\x98"
#define CHECK_SQUARE "\xef\x85\x8a"
#define CHEVRON_DOWN "\xef\x81\xb8"
#define CHEVRON_UP "\xef\x81\xb7"
#define CIRCLE "\xef\x84\x91"
#define MICROCHIP "\xef\x8b\x9b"
#define PLUG "\xef\x87\xa6"
#define RESISTOR "\xef\xbf\xb2"
#define SQUARE "\xef\x83\x88"
#define SWITCH_CLOSED "\xef\xbf\xb1"
#define SWITCH_OPEN "\xef\xbf\xb0"
#define THERMOMETER_FULL "\xef\x8b\x87"
#define UNLOCK_ALT "\xef\x84\xbe"

#define N_SYMBOLS 16
static const char *all_symbols[] = {
    BOLT,
    BROADCAST_TOWER,
    CHECK_CIRCLE,
    CHECK_SQUARE,
    CHEVRON_DOWN,
    CHEVRON_UP,
    CIRCLE,
    MICROCHIP,
    PLUG,
    RESISTOR,
    SQUARE,
    SWITCH_CLOSED,
    SWITCH_OPEN,
    THERMOMETER_FULL,
    UNLOCK_ALT,
};

extern const font_header_t f_font_awesome_5_free;
extern const font_header_t f_roboto;
extern const font_header_t f_roboto_mono;

#endif
