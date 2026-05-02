#pragma once
#include <stdbool.h>
#include <stdint.h>

// This library supports both:
//   * legacy ui_board, dual color LED, no back button, using 8-bit MCP23S08
//   * ui_board_1u, 2x RGB LED, back button, using 16 bit MCP23S17
// Select the flavor at runtime when calling ui_init();
typedef enum { UI_BOARD, UI_BOARD_1U } t_ui_board_type;

// Meaning of the bits in the value returned by get_button_flags()
// It indicates which events happened since the last call
// Instantaneous value (1 = pushed)
#define EV_ENC (1 << 0)   // encoder button
#define EV_BACK (1 << 1)  // back button
// On push
#define EV_ENC_P (1 << 4)
#define EV_BACK_P (1 << 5)
// Short push and release
#define EV_ENC_S (1 << 8)
#define EV_BACK_S (1 << 9)
// Long push and release
#define EV_ENC_L (1 << 12)
#define EV_BACK_L (1 << 13)
// Encoder ticks. Alternative to get_encoder_ticks()
#define EV_ROT_CCW (1 << 16)
#define EV_ROT_CW (1 << 17)

// Call this once to initialize the ui_board
// before calling this:
//   * Initialize the SPI peripheral (10 MHz max.)
//   * Set the RESET pin low for 1 ms, then high and wait another 1 ms
void ui_init(t_ui_board_type value);

// if reset is true, returns number of encoder ticks (and direction) since last call
// if reset is false, returns accumulated encoder ticks
int get_encoder_ticks(bool reset);

// returns the instantaneous state of the encoder and back button (in the 2 LSBs)
// the other bits are used to indicate events. See the EV_ flags above.
unsigned get_event_flags(void);

// Call this in a tight loop.
// for each call, it:
//   * updates the LEDs
//   * reads the inputs (including the encoder)
//   * sends the frame-buffer if it was changed (one row per call)
// returns false when the framebuffer is currently being sent to the OLED.
// to avoid glitches, only draw to the framebuffer after it returns true.
bool ui_board_poll(void);

// # Set the LED status, bits of rgb_value are {B, G, R}
void set_leda(unsigned rgb_value);
void set_ledb(unsigned rgb_value);

// Get raw MCP23 GPIO input values
uint16_t get_gpios(void);
