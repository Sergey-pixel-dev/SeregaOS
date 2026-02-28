#include "../../include/kernel/uart.h"

extern "C" void kernel_main(void)
{
    gpio_uart_init();
    uart_init(PL011_0);
    uart_puts(PL011_0, "Hello from Serega_OS!");
    while (1)
    {
        uart_putc(PL011_0, uart_getc(PL011_0));
    }
}
