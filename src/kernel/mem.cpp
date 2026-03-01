#include "../../include/kernel/mem.h"

extern "C" char __end[]; // add extraction of amount of avalible memory
void PageAllocator::mem_init()
{
    uintptr_t end_addr = (uintptr_t)__end;

    uint32_t mem_size = 128 * 1024 * 1024;
    uint32_t total_pages = mem_size / PAGE_SIZE;
    uint32_t nodes_size = total_pages * sizeof(List<Page *>::Node);
    uint32_t pages_meta_size = total_pages * sizeof(Page);

    // Layout after kernel: [nodes array][page metadata array]
    List<Page *>::Node *nodes = (List<Page *>::Node *)end_addr;
    all_pages_array = (Page *)(end_addr + nodes_size);

    // Account for kernel + metadata
    uintptr_t metadata_end = end_addr + nodes_size + pages_meta_size;
    uint32_t reserved_pages = (metadata_end + PAGE_SIZE - 1) / PAGE_SIZE;

    uint32_t i = 0;
    for (; i < reserved_pages; i++)
    {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_page = 1;
    }
    for (; i < total_pages; i++)
    {
        all_pages_array[i].flags.allocated = 0;
        free_pages.add(all_pages_array + i, &nodes[i]);
    }
}

void bzero(void *ptr, size_t n)
{
    char *ptrc = (char *)ptr;
    for (uint32_t i = 0; i < n; i++)
        ptrc[i] = 0;
}

void *PageAllocator::alloc_page()
{
    if (free_pages.size() == 0)
        return nullptr;
    void *page_mem = (void *)((free_pages.pop()->value - all_pages_array) * PAGE_SIZE);
    bzero(page_mem, PAGE_SIZE);
    return page_mem;
}
void PageAllocator::free_page(void *page_mem)
{
    uintptr_t end_addr = (uintptr_t)&__end;
    uint32_t n_page = (uint64_t)page_mem / PAGE_SIZE;
    List<Page *>::Node *nodes = (List<Page *>::Node *)end_addr;
    free_pages.add(all_pages_array + n_page, &nodes[n_page]);
}
