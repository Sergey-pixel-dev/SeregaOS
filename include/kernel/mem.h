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

class MemAllocator
{
    class Page
    {
    public:
        page_flags_t flags;
    };

    Page *all_pages_array;
    uintptr_t metadata_end;
    List<Page *> free_pages;

    class Segment
    {
    public:
        Segment *prev;
        Segment *next;
        heap_segment_flags_t flags;
        uint32_t size;
    };
    Segment *segments_begin;

public:
    void mem_init();
    void *alloc_page();
    void free_page(void *page_mem);

    void *alloc_segment(size_t size);
    void free_segment(void *segment_mem);
};

#endif