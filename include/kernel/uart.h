#ifndef PL011_H
#define PL011_H

#include "../../include/common/stdio.h"

#define PL011_BASE (0xFE000000 + 0x201000)

typedef struct
{
    uint32_t DR;
    uint32_t RSRECR;
    uint32_t _reserved0[4];
    uint32_t FR;
    uint32_t _reserved1;
    uint32_t ILPR;
    uint32_t IBRD;
    uint32_t FBRD;
    uint32_t LCRH;
    uint32_t CR;
    uint32_t IFLS;
    uint32_t IMSC;
    uint32_t RIS;
    uint32_t MIS;
    uint32_t ICR;
    uint32_t DMACR;
} PL011_TypeDef;

#define PL011_0 ((volatile PL011_TypeDef *)PL011_BASE + 0)
#define PL011_1 ((volatile PL011_TypeDef *)PL011_BASE + 0x400)
#define PL011_2 ((volatile PL011_TypeDef *)PL011_BASE + 0x600)
#define PL011_3 ((volatile PL011_TypeDef *)PL011_BASE + 0x800)
#define PL011_4 ((volatile PL011_TypeDef *)PL011_BASE + 0xa00)

#define PL011_FR_TXFF (1 << 5)
#define PL011_FR_RXFE (1 << 4)
#define PL011_FR_BUSY (1 << 3)

#define PL011_LCRH_FEN (1 << 4)
#define PL011_LCRH_WLEN_8 ((1 << 5) | (1 << 6))

#define PL011_CR_PL011EN (1 << 0)
#define PL011_CR_TXE (1 << 8)
#define PL011_CR_RXE (1 << 9)
#define PL011_CR_UARTEN (1 << 0)

void gpio_uart_init(void);
void uart_init(volatile PL011_TypeDef *uart);
void uart_putc(volatile PL011_TypeDef *uart, uint8_t c);
uint8_t uart_getc(volatile PL011_TypeDef *uart);
void uart_puts(volatile PL011_TypeDef *uart, const char *str);
#endif