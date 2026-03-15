#pragma once

#include <stdbool.h>
#include <stdint.h>

// defines the state of the 2 CS_N pins depending on which chip is selected
typedef enum {
    SELECT_OLED = 0b10,
    SELECT_MCP = 0b01,
    SELECT_NONE = 0b11,
} t_ui_cs_n;

// The functions below need to be implemented in a board-specific .c file.

// Return true as long as a SPI transaction is in progress.
// Will be called from interrupt context.
bool ui_spi_is_busy(void);

// Initiate a blocking 8 bit SPI transaction. Transmit val on SDO, MSB first.
// return received data from SDI. Don't touch the CS_N pin.
uint8_t ui_spi_rx_tx(uint8_t val);

// Set the state of the 2 CS_N pins: bit1: CS_N_MCP, bit0: CS_N_OLED
void ui_set_cs_n(t_ui_cs_n val);

// Return the current state of the 2 CS_N pins. Used in the ISR to safe state.
t_ui_cs_n ui_get_cs_n();

// Set the state of the D_C pin (1 = command, 0 = data for the SSD1322)
void ui_set_dc(bool val);

// Get the state of the MCP23Sxx INT pin. Optional! Only useful in polling mode.
bool ui_get_int(void);

// Return a running number of elapsed milliseconds / system ticks / clock cycles
// used to distinguish between short and long push
unsigned ui_get_cycles(void);

// Min. duration for a long-press event. In [cycles], as defined above.
extern const unsigned ui_t_long_press;

// Enable / disable the MCP23 interrupt
void ui_set_mcp_interrupt_enable(bool val);
