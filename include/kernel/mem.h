#ifndef MEM_H
#define MEM_H

#include "../../include/common/stdint.h"
#include "../../include/common/list.h"

#define PAGE_SIZE 4096

typedef struct
{
    uint8_t allocated : 1;
    uint8_t kernel_page : 1;
    uint32_t reserved : 30;
} page_flags_t;

typedef struct
{
    uint8_t allocated : 1;
    uint32_t reserved : 31;
} heap_segment_flags_t;

struct Page
{
    page_flags_t flags;
};

struct Segment
{
    Segment *prev;
    Segment *next;
    heap_segment_flags_t flags;
    uint32_t size;
};

namespace mem
{
    void init();
    void *alloc_page();
    void free_page(void *page_mem);

    void *alloc_segment(size_t size);
    void free_segment(void *segment_mem);
}

void bzero(void *ptr, size_t n);

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr) noexcept;
void operator delete(void *ptr, size_t) noexcept;
void operator delete[](void *ptr) noexcept;
void operator delete[](void *ptr, size_t) noexcept;

#endif
