#include "../../include/kernel/mem.h"

extern "C" char __end[];
extern "C" char __heap_start[];

static Page *all_pages_array;
static uintptr_t metadata_end;
static List<Page *> free_pages;
static Segment *segments_begin;

void bzero(void *ptr, size_t n)
{
    char *ptrc = (char *)ptr;
    for (size_t i = 0; i < n; i++)
        ptrc[i] = 0;
}

void mem::init()
{
    uintptr_t end_addr = (uintptr_t)__end;

    uint32_t mem_size = 128 * 1024 * 1024;
    uint32_t total_pages = mem_size / PAGE_SIZE;
    uint32_t nodes_size = total_pages * sizeof(List<Page *>::Node);
    uint32_t pages_meta_size = total_pages * sizeof(Page);

    List<Page *>::Node *nodes = (List<Page *>::Node *)end_addr;
    all_pages_array = (Page *)(end_addr + nodes_size);
    metadata_end = end_addr + nodes_size + pages_meta_size;

    uint32_t reserved_pages = (metadata_end + PAGE_SIZE - 1) / PAGE_SIZE;

    uint32_t i = 0;
    for (; i < reserved_pages; i++)
    {
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_page = 1;
    }
    for (; i < total_pages; i++)
    {
        all_pages_array[i].flags.allocated = 0;
        all_pages_array[i].flags.kernel_page = 0;
        free_pages.add(all_pages_array + i, nodes + i);
    }

    // Heap init
    Segment *heap_first_node = (Segment *)__heap_start;
    heap_first_node->size = 1024 * 1024 - sizeof(Segment);
    heap_first_node->flags.allocated = 0;
    heap_first_node->next = nullptr;
    heap_first_node->prev = nullptr;
    segments_begin = heap_first_node;
}

// --- Page allocator ---

void *mem::alloc_page()
{
    if (free_pages.size() == 0)
        return nullptr;

    Page *page = free_pages.pop()->value;
    uint32_t page_index = page - all_pages_array;
    page->flags.allocated = 1;

    void *page_mem = (void *)((uintptr_t)page_index * PAGE_SIZE);
    bzero(page_mem, PAGE_SIZE);
    return page_mem;
}

void mem::free_page(void *page_mem)
{
    uintptr_t end_addr = (uintptr_t)__end;
    uint32_t n_page = (uintptr_t)page_mem / PAGE_SIZE;
    all_pages_array[n_page].flags.allocated = 0;
    all_pages_array[n_page].flags.kernel_page = 0;

    List<Page *>::Node *nodes = (List<Page *>::Node *)end_addr;
    free_pages.add(all_pages_array + n_page, &nodes[n_page]);
}

// --- Heap allocator ---

void *mem::alloc_segment(size_t size)
{
    size = (size + 7) & ~(size_t)7;

    Segment *cur = segments_begin;
    while (cur != nullptr)
    {
        if (!cur->flags.allocated && cur->size >= size)
        {
            if (cur->size > size + sizeof(Segment))
            {
                Segment *new_seg = (Segment *)((uintptr_t)cur + sizeof(Segment) + size);
                new_seg->size = cur->size - size - sizeof(Segment);
                new_seg->flags.allocated = 0;
                new_seg->prev = cur;
                new_seg->next = cur->next;
                if (cur->next != nullptr)
                    cur->next->prev = new_seg;
                cur->next = new_seg;
            }

            cur->size = size;
            cur->flags.allocated = 1;

            void *ptr = (void *)((uintptr_t)cur + sizeof(Segment));
            bzero(ptr, size);
            return ptr;
        }
        cur = cur->next;
    }
    return nullptr;
}

void mem::free_segment(void *segment_mem)
{
    Segment *seg = (Segment *)((uintptr_t)segment_mem - sizeof(Segment));
    seg->flags.allocated = 0;

    if (seg->next != nullptr && !seg->next->flags.allocated)
    {
        seg->size += sizeof(Segment) + seg->next->size;
        seg->next = seg->next->next;
        if (seg->next != nullptr)
            seg->next->prev = seg;
    }

    if (seg->prev != nullptr && !seg->prev->flags.allocated)
    {
        seg->prev->size += sizeof(Segment) + seg->size;
        seg->prev->next = seg->next;
        if (seg->next != nullptr)
            seg->next->prev = seg->prev;
    }
}

// --- Global operators ---

void *operator new(size_t size)
{
    return mem::alloc_segment(size);
}

void *operator new[](size_t size)
{
    return mem::alloc_segment(size);
}

void operator delete(void *ptr) noexcept
{
    mem::free_segment(ptr);
}

void operator delete(void *ptr, size_t) noexcept
{
    mem::free_segment(ptr);
}

void operator delete[](void *ptr) noexcept
{
    mem::free_segment(ptr);
}

void operator delete[](void *ptr, size_t) noexcept
{
    mem::free_segment(ptr);
}
