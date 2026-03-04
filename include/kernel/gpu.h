#ifndef GPU_H
#define GPU_H

#include "../../include/common/stdint.h"

struct Framebuffer
{
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t depth;
    uint8_t *buffer;
};

namespace gpu
{
    bool init(uint32_t width, uint32_t height);

    void put_pixel(uint32_t x, uint32_t y, uint32_t color);
    void fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
    void clear(uint32_t color);

    void put_char(char c, uint32_t color, uint32_t bg);
    void print(const char *str, uint32_t color, uint32_t bg);
    void print(const char *str);

    void set_cursor(uint32_t col, uint32_t row);

    Framebuffer *get_fb();
}

#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0x00FFFFFF
#define COLOR_RED 0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLUE 0x000000FF
#define COLOR_YELLOW 0x00FFFF00
#define COLOR_CYAN 0x0000FFFF

#endif
