#include "textmode.hpp"

namespace ui
{
namespace text
{
	
byte_t* vram = 0xb8000;
byte_t font_color = video::VC_WHITE;
bool initialized = false;
#define CHECK_INIT if (!initialized) return;

struct text
{
	uint16_t columns, rows;
	uint16_t row, col;
	uint8_t tab_size;
	ubyte_t text_color;
	Font::Font_info* font;
}__attribute__((packed)); 
struct text text_mode;

void init(uint16_t pixelW, uint16_t pixelH, ubyte_t default_color, Font::Font_info* font)
{
	text_mode.columns = pixelW / font->distW;
	text_mode.rows    = pixelH / font->distH;
	text_mode.tab_size = 4;
	text_mode.text_color = default_color;
	text_mode.font = font;
	
	initialized = true;
};

void init(uint16_t cols, uint16_t rows, ubyte_t default_color)
{
	text_mode.columns = cols;
	text_mode.rows = rows;
	text_mode.tab_size = 4;
	text_mode.text_color = default_color;
	text_mode.font = nullptr;
	
	initialized = true;
	clear_screen();
	set_foreground_color(default_color);
	set_background_color(default_color);
};

void tm_newline()
{
	CHECK_INIT
	
	text_mode.row++;
	update();
};

void tm_carriage_return()
{
	CHECK_INIT
	
	text_mode.col = 0;
	update();
};

void tm_tab()
{
	CHECK_INIT
	
	if(!text_mode.tab_size)
		return;

	uint16_t _col = (text_mode.col/text_mode.tab_size+1)*text_mode.tab_size;
	while(text_mode.col != _col)
		put_char(' ');
};

void tm_backspace()
{
	CHECK_INIT
	
	if (text_mode.col == 0)
	{
		if (text_mode.row == 0)
		{ }	//already position 0/0, nothing to do here
		else
		{
			text_mode.row--;
			text_mode.col = text_mode.columns - 1;
			video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);
			text_mode.row--;
			text_mode.col = text_mode.columns - 1;
		}
	}
	else
	{
		text_mode.col--;
		video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);
		text_mode.col--;
	}
};

void sxlb_textmoxe_set_cursor(uint16_t row, uint16_t col)
{
	CHECK_INIT
	
	text_mode.row = row;
	text_mode.col = col;
};

void clear_screen()
{
	CHECK_INIT
	
	if (text_mode.font)
		video::clear_screen(video::bg_color);
	else
	{
		int32_t i = ((text_mode.columns*text_mode.rows) << 1) - 1;
		while (i > 0)
		{
			vram[i--] = text_mode.text_color;
			vram[i--] = ' ';
		}
	}
	set_cursor(0, 0);
};

ubyte_t get_background_color()
{
	CHECK_INIT
	
	return text_mode.text_color;
};

void dump_alphabet()
{
	CHECK_INIT
	
	uint8_t i = 255;
	
	while (i--)
		put_char(i);

	put_char('\n');
};

void new_line()
{
	CHECK_INIT
	
	tm_carriage_return();
	tm_newline();
};

uint32_t write_in_line(const char* message, unsigned int line)
{
	CHECK_INIT
	
	set_cursor(0,line);
	write(message);
};

void set_background_color(char bc)
{
	CHECK_INIT
	
	uint32_t i = 0, end = (text_mode.rows * text_mode.columns) << 1;
	bc &= B(11110000);

	while (i != end)
	{
		++i;
		vram[i] &= B(00001111);
		vram[i] |= bc;
		++i;
	}

	text_mode.text_color &= B(00001111);
	text_mode.text_color |= bc;
};

void set_foreground_color(char fc)
{
	CHECK_INIT
	
	uint32_t i = 0, end = (text_mode.rows * text_mode.columns) << 1;
	fc &= B(00001111);

	while (i != end)
	{
		++i;
		vram[i] &= B(11110000);
		vram[i] |= fc;
		++i;
	}

	text_mode.text_color &= B(11110000);
	text_mode.text_color |= fc;
};

/*print_arr32, format e.g.: %u, %i, %x*/
void write_f_array32(const char* format, char_t* data, uint32_t length, uint32_t struct_size, uint32_t struct_offset)
{
	CHECK_INIT
	
#if __CHECKS_NLPTR
	if (!format ||!data)
		return;
#endif

	for (uint32_t i = 0; i < length; i++)
	{
		write_f(format, data[struct_size*i + struct_offset]);
		put_char(' ');
	}
};

/*printlf*/
void write_line_f(const char* args, ...)
{
	CHECK_INIT
	
#if __CHECKS_NLPTR
	if (!args)
		return;
#endif

	va_list ap;
	va_start(ap, args);
	v_write_f(args, ap);
	va_end(ap);
	
	text_mode.col = 0;
	text_mode.row++;
	update();
};

/*printf*/
void write_f(const char* args, ...)
{
	CHECK_INIT
	
#if __CHECKS_NLPTR
	if (!args)
		return;
#endif

	va_list ap;
	va_start(ap, args);
	v_write_f(args, ap);
	va_end(ap);
};

void v_write_f(const char* args, va_list ap)
{
	CHECK_INIT
	
	uint32_t u;
	int32_t i = -1;

	while (args[++i])
	{
		if (args[i] == '%')
		{
			char buffer[20];
			switch (args[++i])
			{
			case 'u':
			{
				u = va_arg(ap, uint32_t);
				sxlb_convert_itoa_u32_dec(u, buffer);
				write(buffer);
			} break;
			case 'i':
			{
				u = va_arg(ap, int32_t);
				sxlb_convert_itoa_s32_dec(u, buffer);
				write(buffer);
			} break;
			case 'x':
			{
				u = va_arg(ap, uint32_t);
				sxlb_convert_itoa_u32_hex(u, buffer);
				write(buffer);
			} break;
			case 'b':
			{
				u = va_arg(ap, uint32_t);		//cant pass a byte, implicit dword
				sxlb_convert_itoa_u8_hex((uint8_t)(0xff & u), buffer);
				write(buffer);
			} break;
			case 's':
			{
				u = va_arg(ap, LPTR);			//cant pass a byte, implicit dword
				write((char_t*)u);//actuallisy is char_t*
			} break;
			case 'm':
			{
				u = va_arg(ap, uint32_t);			//cant pass a byte, implicit dword
				
				if ((u >> 30))
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u >> 30), buffer);
					printf("~%s GiB", buffer);
				}
				else if ((u >> 20))
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u >> 20), buffer);
					printf("~%s MiB", buffer);
				}
				else if ((u >> 10))
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u >> 10), buffer);
					printf("~%s KiB", buffer);
				}
				else
				{
					sxlb_convert_itoa_s16_dec((uint16_t)u, buffer);
					printf("%s B", buffer);
				}
			} break;
			case 'M':
			{
				u = va_arg(ap, uint32_t);		//cant pass a byte, compiler converts to dword
				
				if (u > 1000000000)
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u / 1000000000), buffer);
					printf("%s GB", buffer);
				}
				else if (u > 1000000)
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u / 1000000), buffer);
					printf("%s MB", buffer);
				}
				else if (u > 1000)
				{
					sxlb_convert_itoa_s16_dec((uint16_t)(u / 1000), buffer);
					printf("%s KB", buffer);
				}
				else
				{
					sxlb_convert_itoa_s16_dec((uint16_t)u, buffer);
					printf("%s B", buffer);
				}
			} break;
			default:
				put_char('%');
				put_char('?');
				break;
			}
		}
		else
			put_char(args[i]);
	}
};

void write(const char_t* text)
{
	CHECK_INIT
	
#if __CHECKS_NLPTR
	if (!text)
		return;
#endif

	char c;
	while(c = *text++)
		put_char(c);
};

void writeline(const char_t* text)
{
	CHECK_INIT
	
	//no __CHECKS_NLPTR
	write(text);
	put_char('\n');
};

void put_char(const char c, ubyte_t color)
{
	CHECK_INIT
	
	ubyte_t tmp = text_mode.text_color;
	text_mode.text_color = color;

	put_char(c);
	text_mode.text_color = tmp;
};

void put_char(const char c)
{
	CHECK_INIT
	
	if (c > 32 && c < 127)	//check if its a printable char
	{
		if (text_mode.font)	//write to the vram of the video mode
		{
			video::draw_char(text_mode.col++ * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font, text_mode.text_color, c - 33);
			update();
		}
		else
		{
			vram[(text_mode.col++ + text_mode.row*text_mode.columns) << 1] = c;
			update();
		}
	}
	else
		switch(c)
		{
			case ' ':
				//if (text_mode.font)		//overdraw the space
				//	video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);

				text_mode.col++;
				update();
				break;
			case '\n':
				{
					tm_carriage_return();
					tm_newline();
				} break;
			case '\t':
				{
					tm_tab();
				} break;
			case '\b':
				{
					tm_backspace();
				} break;
			default:
				video::draw_char(text_mode.col++ * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font, text_mode.text_color, 64);	//30 = '?'
				update();
				break;
		}
};

void get_size(uint16_t& cols, uint16_t& rows)
{
	CHECK_INIT
	
	cols = text_mode.columns;
	rows = text_mode.rows;
};

void set_tab_with(uint8_t columns)
{
	CHECK_INIT
	
	text_mode.tab_size = columns;
};

uint8_t get_tab_with()
{
	CHECK_INIT
	return text_mode.tab_size;
};

void update()
{
	CHECK_INIT
	
	while(text_mode.col >= text_mode.columns)
	{
		text_mode.col-=text_mode.columns;
		text_mode.row++;
	}
	if (text_mode.col >= text_mode.columns)
		text_mode.col = 0;
	if(text_mode.row>=text_mode.rows)
		text_mode.row = 0;
	
	if (!text_mode.font)
	{
		uint16_t pos = text_mode.col + text_mode.row*text_mode.rows;

		asm_outb(0x3D4, 0x0F);
		asm_outb(0x3D5, (ubyte_t)(pos & 0xFF));
		asm_outb(0x3D4, 0x0E);
		asm_outb(0x3D5, (ubyte_t)((pos >> 8) & 0xFF));
	}
};


void set_cursor(uint16_t col, uint16_t row)
{
	CHECK_INIT
	
	text_mode.col = col;
	text_mode.row = row;
	update();
};

}
}
