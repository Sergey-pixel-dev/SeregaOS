#include "../../include/kernel/uart.h"
#include "../../include/kernel/mem.h"
#include "../../include/kernel/gpu.h"

extern "C" void kernel_main(void)
{
    gpio_uart_init();
    uart_init(PL011_0);
    uart_puts(PL011_0, "Booting SeregaOS...\r\n");

    mem::init();

    if (gpu::init(1280, 720))
    {
        gpu::print("SeregaOS v0.1\n");
        gpu::print("Framebuffer initialized.\n", COLOR_GREEN, COLOR_BLACK);
        gpu::print("Hello from GPU!\n");
    }
    else
    {
        uart_puts(PL011_0, "GPU init failed!\r\n");
    }

    while (1)
    {
        uart_putc(PL011_0, uart_getc(PL011_0));
    }
}
