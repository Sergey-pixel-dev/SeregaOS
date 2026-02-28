#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"
uint8_t getc()
{
    return uart_getc(PL011_0);
}
void putc(uint8_t c)
{
    uart_putc(PL011_0, c);
}
void puts(const char *s)
{
    uart_puts(PL011_0, s);
}