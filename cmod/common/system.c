#include "demo.h"
#include "frame_buffer.h"
#include "gpio.h"
#include "hardware_interface.h"
#include "irqs.h"
#include "lv_font.h"
#include "print.h"
#include "settings.h"
#include "spi.h"
#include "spi_memio.h"
#include "ssd1322.h"
#include "timer.h"
#include "uart.h"
#include "ui_board.h"
#include <stdbool.h>
#include <stdint.h>

uint32_t *irq(uint32_t *regs, uint32_t irqs) {
  // called for all 32 interrupts
  // *regs = context save X-registers
  // irqs = q1 = bitmask of all IRQs to be handled
  if (irqs & (1 << IRQ_UART0_RX)) {
    switch (UART_GETC(BASE_UART0)) {
    case 0x14:               // Ctrl + T = reset
      _picorv32_irq_reset(); // reboot from interrupt
      break;
    }
  }
  if (irqs & (1 << IRQ_MCP32)) {
    ui_isr();
  }
  return regs;
}

// Disabled here because we use the print function to
// draw text to the display in demo.c
// void _putchar(char c)
// {
//     // hook for all print_* functions
//     UART_PUTC(BASE_UART0, c);
// }

int main(void) {
  UART_INIT(BASE_UART0, BOOTLOADER_BAUDRATE); // Debug print (USB serial)
  _picorv32_irq_enable(1 << IRQ_UART0_RX);

  // Initialize the SPI peripheral
  //           ss_man, ss_ctrl, cpol, cpha, lsb, nbits, clk_div
  SPI_INIT(IO_SPI, 1, 1, 0, 0, 0, 8, IO_SPI_CLKDIV);

  // Initialize CS_N pins
  ui_set_cs_n(SELECT_NONE);
  SET_GPIO1(IO_GPIO, GPIO_OE_REG, IO_CSN, 1);
  SET_GPIO1(IO_GPIO, GPIO_OE_REG, OLED_BIT_CSN, 1);
  SET_GPIO1(IO_GPIO, GPIO_OE_REG, OLED_BIT_D_C, 1);

  // Set the RES_N pin low and release it
  SET_GPIO1(IO_GPIO, GPIO_OUT_REG, IO_RSTN, 0);
  SET_GPIO1(IO_GPIO, GPIO_OE_REG, IO_RSTN, 1);
  DELAY_MS(1);
  // Release the RES_N
  SET_GPIO1(IO_GPIO, GPIO_OUT_REG, IO_RSTN, 1);
  DELAY_MS(1);

  ui_init(UI_BOARD);

  while (1) {
    demo();
    send_fb();
    // DELAY_MS(100);
  }
}
