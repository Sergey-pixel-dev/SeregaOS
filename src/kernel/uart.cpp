#include "../../include/kernel/uart.h"
#include "../../include/kernel/gpio.h"
void gpio_uart_init(void)
{

    uint32_t sel = GPIO->GPFSEL[1];
    sel &= ~((7u << 12) | (7u << 15));
    sel |= ((4u << 12) | (4u << 15));
    GPIO->GPFSEL[1] = sel;

    uint32_t pup = GPIO->PUP_PDN_CNTRL[0];
    pup &= ~((3u << 28) | (3u << 30));
    GPIO->PUP_PDN_CNTRL[0] = pup;
}

void uart_init(volatile PL011_TypeDef *uart)
{
    gpio_uart_init();
    uart->CR = 0;
    uart->ICR = 0x7FF;

    // 115200 бод
    uart->IBRD = 26;
    uart->FBRD = 3;

    uart->LCRH = PL011_LCRH_WLEN_8 | PL011_LCRH_FEN;

    uart->IMSC = 0;

    uart->CR = PL011_CR_UARTEN | PL011_CR_TXE | PL011_CR_RXE;
}

void uart_putc(volatile PL011_TypeDef *uart, uint8_t c)
{
    while (uart->FR & PL011_FR_TXFF)
        ;
    uart->DR = c;
}

uint8_t uart_getc(volatile PL011_TypeDef *uart)
{
    while (uart->FR & PL011_FR_RXFE)
        ;

    return uart->DR;
}

void uart_puts(volatile PL011_TypeDef *uart, const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
        uart_putc(uart, (uint8_t)str[i]);
}