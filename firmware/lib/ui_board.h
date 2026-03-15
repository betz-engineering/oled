#pragma once
#include <stdbool.h>
#include <stdint.h>

// This library supports both, legacy ui_board, using 8-bit MCP23S08 and
// ui_board_1u, using 16 bit MCP23S17. Use this define to enable legacy mode,
// where only the color of 1 LED can be set to green, red or orange
// #define UI_BOARD_LEGACY

// Meaning of the bits in the value returned by get_button_flags()
// It indicates which events happened since the last call
#define EV_ENC (1 << 0)     // Current state of the encoder knob (1 = pushed)
#define EV_BACK (1 << 1)    // Current state of the back button (1 = pushed)
#define EV_ENC_S (1 << 4)   // A short push and release of the encoder knob happened
#define EV_BACK_S (1 << 5)  // A short push and release of the back button happened
#define EV_ENC_L (1 << 8)   // A long push and release of the encoder knob happened
#define EV_BACK_L (1 << 9)  // A long push and release of the back button happened

// Supported hardware flavors
typedef enum { UI_BOARD, UI_BOARD_1U } t_ui_board_type;

// Register this function to be called in an interrupt on a rising edge of the MCP23Sxx INT pin
// or call it really fast in a loop (at least 200 Hz)
void ui_isr(void);

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
unsigned get_button_flags(void);

// # Set the LED status, bits of rgb_value are {B, G, R}
void set_leda(unsigned rgb_value);
void set_ledb(unsigned rgb_value);

// Get raw MCP23 GPIO input values
uint16_t get_gpios(void);
