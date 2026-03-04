#include "../../include/kernel/mailbox.h"

static inline void mmio_write(uintptr_t reg, uint32_t val)
{
    *(volatile uint32_t *)reg = val;
}

static inline uint32_t mmio_read(uintptr_t reg)
{
    return *(volatile uint32_t *)reg;
}

volatile uint32_t mailbox::buffer[36] __attribute__((aligned(16)));

bool mailbox::call(uint8_t channel)
{
    uint32_t addr = (uint32_t)(uintptr_t)&buffer[0];
    uint32_t msg = (addr & ~0xF) | (channel & 0xF);

    while (mmio_read(MAILBOX_STATUS) & MAILBOX_FULL)
        ;

    mmio_write(MAILBOX_WRITE, msg);

    while (true)
    {
        while (mmio_read(MAILBOX_STATUS) & MAILBOX_EMPTY)
            ;
        uint32_t response = mmio_read(MAILBOX_READ);
        if ((response & 0xF) == channel)
            return (buffer[1] == MAILBOX_RESPONSE_OK);
    }
}
