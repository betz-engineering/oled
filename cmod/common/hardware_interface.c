// SOC-specific implementation of the definitions in hardware_interface.h
#include <hardware_interface.h>
#include "gpio.h"
#include "irqs.h"
#include "settings.h"
#include "spi.h"
#include "timer.h"

// Initiate a blocking 8 bit SPI transaction. Transmit val on SDO, MSB first.
// return received data from SDI.
// Used for sending configuration and reading the MCP23
// Don't touch the CS_N pin.
uint8_t ui_spi_rx_tx(uint8_t val) {
    SPI_SET_DAT(IO_SPI, val);
    while (SPI_IS_BUSY(IO_SPI))
        ;
    return SPI_GET_DAT(IO_SPI);
}

// Send framebuffer data to the OLED.
// Blocking transfer. It's the simplest way.
void ui_spi_tx_chunk(uint8_t *buf, unsigned len) {
    while (len-- > 0)
        ui_spi_rx_tx(*buf++);
}

// Return true as long as the above SPI transmission is in progress.
bool ui_spi_is_busy(void) { return SPI_IS_BUSY(IO_SPI); }

// Set the state of the 2 CS_N pins: bit1: CS_N_MCP, bit0: CS_N_OLED
void ui_set_cs_n(t_ui_cs_n val) {
    SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, OLED_BIT_CSN, val & 1);
    SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, IO_CSN, val & 2);
}

// Set the state of the D_C pin (1 = command, 0 = data for the SSD1322)
void ui_set_dc(bool val) { SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, OLED_BIT_D_C, val); }

// Get the state of the MCP23Sxx INT pin
bool ui_get_int(void) { return GET_GPIO1(BASE_GPIO, GPIO_IN_REG, IO_INT); }

// Return a running number of elapsed milliseconds / system ticks / clock cycles
// used to distinguish between short and long push
unsigned ui_get_cycles(void) { return getCycles(); }

// Half a second at 100 MHz
const unsigned ui_t_long_press = 50000000;
