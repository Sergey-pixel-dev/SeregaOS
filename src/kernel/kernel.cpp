#include "../../include/kernel/uart.h"
#include "../../include/kernel/mem.h"

PageAllocator page_allocator;

extern "C" void kernel_main(void)
{
    page_allocator.mem_init();
    void *ptr = page_allocator.alloc_page();
    gpio_uart_init();
    uart_init(PL011_0);
    uart_puts(PL011_0, "Hello from Serega_OS!");
    page_allocator.free_page(ptr);
    while (1)
    {
        uart_putc(PL011_0, uart_getc(PL011_0));
    }
}
