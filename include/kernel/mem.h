#ifndef MEM_H
#define MEM_H

#include "../../include/common/stdint.h"
#include "../../include/common/list.h"

#define PAGE_SIZE 4096

typedef struct
{
    uint8_t allocated : 1;   // This page is allocated to something
    uint8_t kernel_page : 1; // This page is a part of the kernel
    uint32_t reserved : 30;
} page_flags_t;

class PageAllocator
{
    // Current memory structure: |.text .data .bss .... (PageAllocator etc. are saved in .bss section) | |node 0, node 1, ....
    // .. (nodes save pointers to next\prev nodes and pointer to page metadata structure, so we need allocate all possible nodes even if they point to already allocated page) |
    // |page metada 0, page metadata 1, ..... | |page 4Kb, page 4Kb, ... |

    // We use linked list for saving free pages.
    // But we can implement priority for allocate pages (for example, the nearest).
    // Idea for anlysis for kursovoy project.
    class Page
    {
    public:
        uint64_t vaddr_mapped;
        page_flags_t flags;
    };
    Page *all_pages_array;
    List<Page *> free_pages;

public:
    void mem_init();
    void *alloc_page();
    void free_page(void *page_mem);
};
#endif