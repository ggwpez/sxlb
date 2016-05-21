#include "video.hpp"
#include "../io/hwaccess.hpp"

namespace2(ui, video)
{
    struct video_init_t vdata;

    rgba_t bg_color;

    void init(struct video_init_t* data)
    {
        vdata = *data;
    }

    void update()
    {
        if (0)
        {	//wait for vertical retrace
            while ((io::asm_inb(0x03da) & 0x08));
            while (!(io::asm_inb(0x03da) & 0x08));
        }

        //memory::memcpy(vram, zbuffer, 64000);
    }

    int p(int b, int e)
    {
        int res = 1;
        while (e--)
            res *= b;

        return res;
    }

    void test()
    {
        int c = 0;
        int o = p(2, vdata.bpp) / ((vdata.w /10) *(vdata.h /10));
        for (int x = 0; x < 800; x += 10)
            for (int y = 0; y < 600; y += 10)
                ui::video::draw_rect_filled(x, y, 10, 10, c += o);
    }

    void inline draw_pixel(uint16_t x, uint16_t y, rgba_t c)
    {
        if (x >= vdata.w || y >= vdata.h)
            return;

        LPTR addr = vdata.fb +vdata.pitch *y +(x *vdata.bypp);

        if (vdata.bypp == 1)
        {
            uint8_t* p = addr;
            c = clr_32_to_8(c);
            *p = c;
        }
        else if (vdata.bypp == 2)
        {
            uint16_t* p = addr;
            c = clr_32_to_16(c);
            *(uint16_t*)addr = c;
        }
        else if (vdata.bypp == 3)
        {
            uint32_t* p = addr;
            c = clr_32_to_24(c);
            *p = (c & 0xffffff) | (*p & 0xff000000);
        }
        else if (vdata.bypp = 4)
        {
            uint32_t* p = addr;
            *p = c;
        }
    }

    void clr()
    {
        fill_s(0x00000000);
    }

    void fill_s(rgba_t c)
    {
        for (uint16_t x = 0; x < vdata.w; x++)
            for (uint16_t y = 0; y < vdata.h; y++)
                draw_pixel(x, y, c);
    }

    void draw_char(uint16_t x, uint16_t y, Font::Font_info* font, rgba_t color, char c)
    {
        uint8_t* mask = font->mask + c * 11;
        //video::draw_rect_filled(x,y, font->H, font->W, bg_color); 	//whipe out the current letter

        for (size_t h = 0; h < font->H; h++)
            for (size_t w = font->W; w > 0; w--)
                if (mask[h] & (1 << w))
                    draw_pixel((font->W - w) + x, y + h, color);
    };

    void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, rgba_t color)
    {
        draw_line(x, y, x +width, y, color);
        draw_line(x, y, x, y +heigth, color);
        draw_line(x +width, y, x +width, y +heigth, color);
        draw_line(x, y +heigth, x +width, y +heigth, color);
    }

    void draw_rect_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, rgba_t color)
    {
        for (uint16_t px = x; px < x +width; px++)
            for (uint16_t py = y; py < y +heigth; py++)
                draw_pixel(px, py, color);
    }

    void draw_polygons(uint32_t num_vertices, uint16_t* vertices, rgba_t color)
    {
        for (uint32_t i = 0; i < num_vertices - 1; i++)
        {
            draw_line(vertices[(i << 1) + 0],
                vertices[(i << 1) + 1],
                vertices[(i << 1) + 2],
                vertices[(i << 1) + 3],
                color);
        }

        draw_line(vertices[0],
            vertices[1],
            vertices[(num_vertices << 1) - 2],
            vertices[(num_vertices << 1) - 1],
            color);
    }

    void draw_triangle(uint16_t* vertices, rgba_t color)
    {
        draw_line(vertices[0], vertices[1], vertices[2], vertices[3], color);
        draw_line(vertices[2], vertices[3], vertices[4], vertices[5], color);
        draw_line(vertices[4], vertices[5], vertices[0], vertices[1], color);
    }

    void draw_line_h(uint16_t x1, uint32_t x2, uint32_t y, rgba_t color)
    {
        if (x2 >= x1)
        {
            for (; x2 >= x1; x1++)
                draw_pixel(x1, y, color);
        }
        else
        {
            for (; x1 > x2; x2++)
                draw_pixel(x2, y, color);
        }
    }

    void draw_line_v(uint16_t x, uint32_t y1, uint32_t y2, rgba_t color)
    {
        if (y2 >= y1)
        {
            for (; y2 >= y1; y1++)
                draw_pixel(x, y1, color);
        }
        else
        {
            for (; y1 > y2; y2++)
                draw_pixel(x, y2, color);
        }
    }

    /*Bresenham alg from: http://www.brackeen.com/vga/source/djgpp20/lines.c.html */
    void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, rgba_t color)
    {
        if (x1 == x2)
        {
            draw_line_v(x1, y1, y2, color);
            return;
        }
        else if (y1 == y2)
        {
            draw_line_h(x1, x2, y1, color);
            return;
        }

        int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

        dx = x2 - x1;      /* the horizontal distance of the line */
        dy = y2 - y1;      /* the vertical distance of the line */
        dxabs = abs(dx);
        dyabs = abs(dy);
        sdx = sgn(dx);
        sdy = sgn(dy);
        x = dyabs >> 1;
        y = dxabs >> 1;
        px = x1;
        py = y1;

        draw_pixel(px, py, color);

        if (dxabs >= dyabs) /* the line is more horizontal than vertical */
        {
            for (i = 0; i < dxabs; i++)
            {
                y += dyabs;
                if (y >= dxabs)
                {
                    y -= dxabs;
                    py += sdy;
                }
                px += sdx;
                draw_pixel(px, py, color);
            }
        }
        else /* the line is more vertical than horizontal */
        {
            for (i = 0; i < dyabs; i++)
            {
                x += dxabs;
                if (x >= dyabs)
                {
                    x -= dyabs;
                    px += sdx;
                }
                py += sdy;
                draw_pixel(px, py, color);
            }
        }
    }

    void draw_circle(uint16_t xc, uint16_t yc, uint16_t r, rgba_t color)
    {
        int d,x,y;
        d=3-2*r;
        x=0;
        y=r;

        while(x<=y)
        {
            draw_pixel(xc+x,yc+y,color);
            draw_pixel(xc-y,yc-x,color);
            draw_pixel(xc+y,yc-x,color);
            draw_pixel(xc-y,yc+x,color);
            draw_pixel(xc+y,yc+x,color);
            draw_pixel(xc-x,yc-y,color);
            draw_pixel(xc+x,yc-y,color);
            draw_pixel(xc-x,yc+y,color);

            if(d<=0)
            {
                d=d+4*x+6;
            }
            else
            {
                d=d+4*x-4*y+10;
                y=y-1;
            }
                x=x+1;
        }
    }
}}
