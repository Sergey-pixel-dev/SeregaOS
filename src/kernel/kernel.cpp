#include "../../include/kernel/uart.h"
#include "../../include/kernel/mem.h"

MemAllocator mem_allocator;

extern "C" void kernel_main(void)
{
    mem_allocator.mem_init();
    void *ptr = mem_allocator.alloc_page();
    int *a = (int *)mem_allocator.alloc_segment(4);
    gpio_uart_init();
    uart_init(PL011_0);
    uart_puts(PL011_0, "Hello from Serega_OS!");
    mem_allocator.free_page(ptr);
    mem_allocator.free_segment((void *)a);
    while (1)
    {
        uart_putc(PL011_0, uart_getc(PL011_0));
    }
}
