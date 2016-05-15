#ifndef VIDEO__HPP
#define VIDEO__HPP

#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../font.hpp"

#define clr_32_to_24(color) (color & 0x00ffffff)
#define clr_32_to_16(color) (color & 0x0000ffff)
#define clr_32_to_8(color) (color & 0x000000ff)

typedef uint32_t rgba_t;
namespace2(ui, video)
{
    extern rgba_t bg_color;

    struct video_init_t
    {
        uint16_t w, h;
        uint8_t bpp, bypp;
        uint32_t pitch;
        LPTR fb;
    };

    void init(video_init_t* data);
    void clr();
    void draw_char(uint16_t x, uint16_t y, Font::Font_info* font, rgba_t color, char c);
    void fill_s(rgba_t c);
    void update();

    void draw_pixel(uint16_t x, uint16_t y, rgba_t color);
    void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, rgba_t color);
    void draw_rect_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, rgba_t color);
    void draw_polygons(uint32_t num_vertices, uint16_t* vertices, rgba_t color);
    void draw_triangle(uint16_t* vertices, rgba_t color);
    void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgba_t color);
}}

#endif // VIDEO__HPP
