#ifndef MAILBOX_H
#define MAILBOX_H

#include "../../include/common/stdint.h"

#define PERIPHERAL_BASE 0xFE000000
#define MAILBOX_BASE (PERIPHERAL_BASE + 0xB880)

#define MAILBOX_READ (MAILBOX_BASE + 0x00)
#define MAILBOX_STATUS (MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE (MAILBOX_BASE + 0x20)

#define MAILBOX_FULL (1 << 31)
#define MAILBOX_EMPTY (1 << 30)

#define MAILBOX_CH_PROP 8

// Property tag IDs
#define TAG_SET_PHYS_WH 0x00048003
#define TAG_SET_VIRT_WH 0x00048004
#define TAG_SET_VIRT_OFFSET 0x00048009
#define TAG_SET_DEPTH 0x00048005
#define TAG_SET_PIXEL_ORDER 0x00048006
#define TAG_ALLOCATE_FB 0x00040001
#define TAG_GET_PITCH 0x00040008
#define TAG_END 0x00000000

// Request/response codes
#define MAILBOX_REQUEST 0x00000000
#define MAILBOX_RESPONSE_OK 0x80000000

namespace mailbox
{
    bool call(uint8_t channel);
    extern volatile uint32_t buffer[36] __attribute__((aligned(16)));
}

#endif
