#ifndef PL011_H
#define PL011_H

#include <stdint.h>
#define PERIPHERAL_BASE 0xFE000000

#define AUX_BASE (PERIPHERAL_BASE + 0x215000)

typedef struct
{
    uint32_t IRQ;
    uint32_t ENABLES;
    uint32_t _reserved0[14];
    uint32_t MU_IO;
    uint32_t MU_IER;
    uint32_t MU_IIR;
    uint32_t MU_LCR;
    uint32_t MU_MCR;
    uint32_t MU_LSR;
    uint32_t MU_MSR;
    uint32_t MU_SCRATCH;
    uint32_t MU_CNTL;
    uint32_t MU_STAT;
    uint32_t MU_BAUD;
} AUX_TypeDef;

#define AUX ((volatile AUX_TypeDef *)AUX_BASE)

#define AUX_ENABLES_MU (1 << 0)
#define AUX_ENABLES_SPI1 (1 << 1)
#define AUX_ENABLES_SPI2 (1 << 2)

#define AUX_MU_LSR_TX_EMPTY (1 << 5)
#define AUX_MU_LSR_RX_READY (1 << 0)

#define AUX_MU_LCR_8BIT (3 << 0)

#define AUX_MU_CNTL_RX_EN (1 << 0)
#define AUX_MU_CNTL_TX_EN (1 << 1)

#endif