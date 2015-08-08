#include "video.hpp"
#include "../system/system.hpp"

namespace ui
{
namespace video
{
	uint16_t screen_width, screen_heigth, size;
	static byte_t* vram = 0xa0000;	//actuallisy is far pointer (ubyte_t far* 0xA0000000)
	static byte_t* zbuffer;
	static bool v_sync_enabled;
	ubyte_t bg_color;

	void init(uint16_t width, uint16_t heigth, ubyte_t background_color, LPTR zBuffer, bool v_sync)
	{
		screen_width = width;
		screen_heigth = heigth;
		size = screen_width * screen_heigth;

		bg_color = background_color;
		zbuffer = zBuffer;
		v_sync_enabled = v_sync;

		clear_screen(bg_color);
	};

	void get_size(uint16_t& width, uint16_t& heigth)
	{
		width = screen_width;
		heigth = screen_heigth;
	};

	/*Waits for v_sync and overrides VRAM with zBuffer*/
	void update()
	{
		if (v_sync_enabled)
		{	//wait for vertical retrace
            while ((hw::asm_inb(0x03da) & 0x08));
            while (!(hw::asm_inb(0x03da) & 0x08));
		}

        memory::memcpy(vram, zbuffer, 64000);
	};

	/*Clears zBuffer*/
	void clear_screen(uint16_t color)
	{
		uint16_t size_tmp = size +1;
		byte_t* tmp_pointer = zbuffer;

		if (color == VC_DEFAULT)
			color = bg_color;

		while (--size_tmp)
			*(tmp_pointer++) = color;
	};

	void draw_pixel(uint16_t x, uint16_t y, ubyte_t color)
	{
		*(zbuffer + x + (y << 8) + (y << 6)) = color;	//= x + y * 320
	};

	void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, ubyte_t color)
	{
		uint32_t tmp;
		width += x;
		heigth += y;

		if (y > heigth)
		{
			tmp = y;
			y = heigth;
			heigth = tmp;
		}
		if (x > width)
		{
			tmp = x;
			x = width;
			width = tmp;
		}

		uint16_t y_offset = (y << 8) + (y << 6);
		uint16_t heigth_offset = (heigth << 8) + (heigth << 6), i;

		for (i = x; i <= width; i++)
		{
			zbuffer[y_offset + i] = color;
			zbuffer[heigth_offset + i] = color;
		}
		for (i = y_offset; i <= heigth_offset; i += screen_width)
		{
			zbuffer[x + i] = color;
			zbuffer[width + i] = color;
		}
	};

	void draw_rect_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, ubyte_t color)
	{
		uint16_t tmp = heigth;

		while (width)
		{
			heigth = tmp;
			while (heigth)
			{
				*(zbuffer + x + width + ((y + heigth) << 8) + ((y + heigth) << 6)) = color;	//= x + y * 320

				heigth--;
			}

			width--;
		}
	};

	void draw_polygons(uint32_t num_vertices, uint16_t* vertices, byte_t color)
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

	/*Vertices count = 6.*/
	void draw_triangle(uint16_t* vertices, byte_t color)
	{
		draw_line(vertices[0], vertices[1], vertices[2], vertices[3], color);
		draw_line(vertices[2], vertices[3], vertices[4], vertices[5], color);
		draw_line(vertices[4], vertices[5], vertices[0], vertices[1], color);
	};

	/*Bresenham alg from: http://www.brackeen.com/vga/source/djgpp20/lines.c.html*/
	void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ubyte_t color)
	{
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

		zbuffer[(py << 8) + (py << 6) + px] = color;

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
	};

	void draw_char(uint16_t x, uint16_t y, Font::Font_info* font, ubyte_t color, char c)
	{
		uint8_t* mask = font->mask + c * 11;
		//video::draw_rect_filled(x,y, font->H, font->W, bg_color); 	//whipe out the current letter

		for (size_t l = 0; l < font->H; l++)
			for (size_t w = font->H; w > 0; w--)
				if (mask[l] & (1 << w))
					draw_pixel((font->W - w) + x, y + l, color);
	};
}
}
