// SOC-specific implementation of the definitions in hardware_interface.h
#include "hardware_interface.h"
#include "gpio.h"
#include "irqs.h"
#include "settings.h"
#include "spi.h"
#include "timer.h"

bool ui_spi_is_busy(void) { return SPI_IS_BUSY(IO_SPI); }

uint8_t ui_spi_rx_tx(uint8_t val) {
    SPI_SET_DAT(IO_SPI, val);
    while (ui_spi_is_busy())
        ;
    return SPI_GET_DAT(IO_SPI);
}

void ui_set_cs_n(t_ui_cs_n val) {
    SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, OLED_BIT_CSN, val & 1);
    SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, IO_CSN, val & 2);
}

t_ui_cs_n ui_get_cs_n() {
    t_ui_cs_n ret = 0;
    if (GET_GPIO1(BASE_GPIO, GPIO_OUT_REG, OLED_BIT_CSN))
        ret = 1;
    if (GET_GPIO1(BASE_GPIO, GPIO_OUT_REG, IO_CSN))
        ret |= 2;
    return ret;
}

void ui_set_dc(bool val) { SET_GPIO1(BASE_GPIO, GPIO_OUT_REG, OLED_BIT_D_C, val); };

unsigned ui_get_cycles(void) { return getCycles(); };

// Half a second at 100 MHz
const unsigned ui_t_long_press = 50000000;

void ui_set_mcp_interrupt_enable(bool val) {
    if (val)
        _picorv32_irq_enable(1 << IRQ_MCP32);
    else
        _picorv32_irq_disable(1 << IRQ_MCP32);
};
