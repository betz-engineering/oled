#pragma once

#include <stdbool.h>
#include <stdint.h>

// defines the state of the 2 CS_N pins depending on which chip is selected
typedef enum { SELECT_OLED = 2, SELECT_MCP = 1, SELECT_NONE = 3 } t_ui_cs_n;

// The functions below need to be implemented in a board-specific .c file.

// Initiate a blocking 8 bit SPI transaction. Transmit val on SDO, MSB first.
// return received data from SDI.
// Used for sending configuration and reading the MCP23
// Don't touch the CS_N pin.
uint8_t ui_spi_rx_tx(uint8_t val);

// Initiate a non-blocking N bit SPI transmission.
// Used to send framebuffer data to the OLED.
// Ideally this is implemented with DMA or interrupts if possible.
// Otherwise a blocking transfer is also okay.
// Don't touch the CS_N pins.
void ui_spi_tx_chunk(uint8_t *buf, unsigned len);

// Return true as long as the above SPI transmission is in progress.
bool ui_spi_is_busy(void);

// Set the state of the 2 CS_N pins: bit1: CS_N_MCP, bit0: CS_N_OLED
void ui_set_cs_n(t_ui_cs_n val);

// Set the state of the D_C pin (1 = command, 0 = data for the SSD1322)
void ui_set_dc(bool val);

// Get the state of the MCP23Sxx INT pin
bool ui_get_int(void);

// Return a running number of elapsed milliseconds / system ticks / clock cycles
// used to distinguish between short and long push
unsigned ui_get_cycles(void);

// Min. duration for a long-press event. In [cycles], as defined above.
extern const unsigned ui_t_long_press;
