#include "demo.h"
#include "frame_buffer.h"
#include "gpio.h"
#include <hardware_interface.h>
#include "irqs.h"
#include "font.h"
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
#include <stdio.h>


uint32_t *irq(uint32_t *regs, uint32_t irqs) {
    // called for all 32 interrupts
    // *regs = context save X-registers
    // irqs = q1 = bitmask of all IRQs to be handled
    if (irqs & (1 << IRQ_UART0_RX)) {
        switch (UART_GETC(BASE_UART0)) {
        case 0x14:                  // Ctrl + T = reset
            _picorv32_irq_reset();  // reboot from interrupt
            break;
        }
    }
    // UI_ISR is not needed to trigger an interrupt anymore
    // if (irqs & (1 << IRQ_MCP32)) {
    //   ;
    // }
    return regs;
}

// character output function for printf
static int uart_putc(char c, FILE *file) {
    (void)file;
    UART_PUTC(BASE_UART0, c);
    return c;
}

// Set up the stream for writing
static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putc, NULL, NULL, _FDEV_SETUP_WRITE);

// Define the global streams picolibc is looking for
FILE *const stdout = &uart_stdout;
FILE *const stderr = &uart_stdout;


int main(void) {
    UART_INIT(BASE_UART0, BOOTLOADER_BAUDRATE);  // Debug print (USB serial)
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

    ui_init(UI_BOARD_1U);

    printf("Hi, this is UI_BOARD demo on CMODA7\n");

    while (1) {
        bool next_frame = ui_board_poll();
        if (next_frame)
            demo();
    }
}
