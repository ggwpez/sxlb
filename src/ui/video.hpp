#ifndef VIDEO__HPP
#define VIDEO__HPP

#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../font.hpp"

#define CLR_32_24(c) (c & 0x00ffffff)
#define CLR_32_16(c)  ((((CLR_32_24(c) >> 16) >> 3) << 11) | \
                          (((CLR_32_24(c) >>  8) >> 2) << 5) | \
                          (((CLR_32_24(c) >>  0) >> 3) << 0))//using 5:6:5
#define CLR_32_8(c) (c & 0x000000ff)
#define CLR_32_4(c) (clr44_t)(c ? CLR_32_24(c) /16777216.0f : 0)


// 24: rrrrrrrrggggggggbbbbbbbb
// 16:         rrrrrggggggbbbbb

typedef uint32_t clr32_t;
typedef uint16_t clr16_t, clr15_t;
typedef  uint8_t clr8_t, clr4_t, clr44_t;
namespace2(ui, video)
{
    extern clr32_t bg_color;

    struct video_init_t
    {
        uint16_t w, h;
        uint8_t bpp, bypp;
        uint32_t pitch;
        LPTR fb;
        uint32_t len;
        uint8_t type;
    };

    void init(video_init_t* data);
    void clr();
    void draw_char(uint16_t x, uint16_t y, Font::Font_info* font, clr32_t color, char c);
    void fill_s(clr32_t c);
    void update();
    void test();

    void draw_pixel(uint16_t x, uint16_t y, clr32_t color);
    void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, clr32_t color);
    void draw_rect_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, clr32_t color);
    void draw_polygons(uint32_t num_vertices, uint16_t* vertices, clr32_t color);
    void draw_triangle(uint16_t* vertices, clr32_t color);
    void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, clr32_t color);
    void draw_line_h(uint16_t x1, uint32_t x2, uint32_t y, clr32_t color);
    void draw_line_v(uint16_t x, uint32_t y1, uint32_t y2, clr32_t color);
    void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, clr32_t color);
}}

#endif // VIDEO__HPP
