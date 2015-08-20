#pragma once

#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../font.hpp"

namespace2(ui, video)
{
	/*You can pick one of these pre-defined colors, or yust take any number from 0-255*/
	enum video_color
	{
		VC_BLACK = 0,
		VC_BLUE = 1,
		VC_GREEN = 2,
		VC_CYAN = 3,
		VC_RED = 4,
		VC_MAGENTA = 5,
		VC_BROWN = 6,
		VC_LIGHTGREY = 7,
		VC_DARKGRAY = 8,
		VC_LIGHTBLUE = 9,
		VC_LIGHTGREEN = 10,
		VC_LIGHTCYAN = 11,
		VC_LIGHTRED = 12,
		VC_LIGHTMAGENTA = 13,
		VC_YELLOW = 14,
		VC_WHITE = 15,
		VC_DEFAULT = 256
	};

	/*Only supports 1 byte per pixel*/
	void init(uint16_t width, uint16_t heigth, ubyte_t background_color, LPTR zBuffer, bool v_sync);
	void update();
	void get_size(uint16_t& width, uint16_t& heigth);
	void clear_screen(uint16_t color);
	void draw_pixel(uint16_t x, uint16_t y, ubyte_t color);
	void draw_char(uint16_t x, uint16_t y, Font::Font_info* font, ubyte_t color, char c);
	void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, ubyte_t color);
	void draw_rect_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, ubyte_t color);
	void draw_polygons(uint32_t num_vertices, uint16_t* vertices, byte_t color);
	void draw_triangle(uint16_t* vertices, byte_t color);
	void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ubyte_t color);

	extern ubyte_t bg_color;
}
}
