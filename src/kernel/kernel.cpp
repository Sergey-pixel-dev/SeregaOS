#include "../../include/kernel/uart.h"
#include "../../include/kernel/mem.h"

extern "C" void kernel_main(void)
{
    mem::init();
    void *ptr = mem::alloc_page();
    int *a = (int *)mem::alloc_segment(4);
    char *str = new char[120];
    gpio_uart_init();
    uart_init(PL011_0);
    uart_puts(PL011_0, "Hello from Serega_OS!");
    mem::free_page(ptr);
    mem::free_segment((void *)a);
    delete[] str;
    while (1)
    {
        uart_putc(PL011_0, uart_getc(PL011_0));
    }
}
