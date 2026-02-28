#ifndef GPIO_H
#define GPIO_H

#include "../../include/common/stdio.h"

#define GPIO_BASE (0xFE000000 + 0x200000)

typedef struct
{
    uint32_t GPFSEL[6];
    uint32_t _reserved0;
    uint32_t GPSET[2];
    uint32_t _reserved1;
    uint32_t GPCLR[2];
    uint32_t _reserved2;
    uint32_t GPLEV[2];
    uint32_t _reserved3;
    uint32_t GPEDS[2];
    uint32_t _reserved4;
    uint32_t GPREN[2];
    uint32_t _reserved5;
    uint32_t GPFEN[2];
    uint32_t _reserved6;
    uint32_t GPHEN[2];
    uint32_t _reserved7;
    uint32_t GPLEN[2];
    uint32_t _reserved8;
    uint32_t GPAREN[2];
    uint32_t _reserved9;
    uint32_t GPAFEN[2];
    uint32_t _reserved10[21];
    uint32_t PUP_PDN_CNTRL[4];
} GPIO_TypeDef;

#define GPIO ((volatile GPIO_TypeDef *)GPIO_BASE)

#endif