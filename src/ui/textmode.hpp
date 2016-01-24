#pragma once

#include "../types.hpp"
#include "../sprintf.hpp"
#include "../io/hwaccess.hpp"
#include "../convert.hpp"
#include "../ui/video.hpp"

#define print 			(ui::text::write)
#define printl 			(ui::text::write_line)
#define printf          (ui::text::write_f)
#define printfl 		(ui::text::write_line_f)
#define putc 			(ui::text::put_char)
#define vprintf 		(ui::text::v_write_f)
#define printf_arr32 	(ui::text::write_f_array32)

#define LOG
#ifdef LOG
#define logINF          ui::text::set_color_reset(); ui::text::write_f
#define logDBG          ui::text::set_fc(FC_BLUE);  ui::text::write_f
#define logOK           ui::text::set_fc(FC_GREEN);  ui::text::write_f
#define logWAR          ui::text::set_fc(FC_MAGENTA); ui::text::write_f
#define logERR          ui::text::set_fc(FC_RED); ui::text::write_f
#define logDONE         logOK("done\n"); ui::text::set_color_reset();
#endif
#ifndef LOG
#define logINF
#define logDBG
#define logOK
#define logWAR
#define logERR
#define logDONE
#endif

enum background_color : uchar_t
{
    BC_BLACK			= 0,
    BC_LIGHTBLUE		= 1 << 4,
    BC_LIGHTGREEN		= 2 << 4,
    BC_LIGHTTURQUOISE	= 3 << 4,
    BC_LIGHTRED			= 4 << 4,
    BC_LIGHTMAGENTA		= 5 << 4,
    BC_ORANGE			= 6 << 4,
    BC_WHITE			= 7 << 4,
    BC_MOD_BLINKING		= 8 << 4	//modifier-bit for blinking symols
};

enum foreground_color : uchar_t
{
    FC_BLACK			= 0,
    FC_BLUE				= 1,
    FC_GREEN			= 2,
    FC_TURQUOISE		= 3,
    FC_RED				= 4,
    FC_MAGENTA			= 5,
    FC_BROWN			= 6,
    FC_LIGHTGRAY		= 7,
    FC_MOD_INTENSITY	= 8			//modifier-bit for more intense symols
};

namespace2(ui, text)
{
    extern byte_t* vram;
    extern bool initialized;
    void		write						(const char_t* text);
    void		update                      ();
    void		put_char					(char c);
    void		put_char					(char c, ubyte_t color);
    void 		get_size                    (uint16_t& cols, uint16_t& rows);
    void		writeline					(const char_t* text);
    void		v_write_f					(const char* args, va_list ap);
    void		set_cursor                  (uint16_t col, uint16_t row);
    uint8_t		get_tab_with				();
    void		set_tab_with				(uint8_t rows);
    void		clear_screen				();
    void		init						(uint16_t pixelW, uint16_t pixelH, ubyte_t default_color, Font::Font_info* font);
    void		init						(uint16_t cols, uint16_t rows, ubyte_t default_color);
    void        write_line                  (const char* text);
    void		write_line_f				(const char* args, ...);
    uint32_t	write_in_line				(const char* message, unsigned int line);
    void		dump_alphabet				();
    /*e.g.: printf_arr32("%u ", array, array.size, 10, 2). The last parameter can be used to print only the ints from an struct array, then you just give the offset from the base of the struct to the needed int and the size of the struct.*/
    void		write_f_array32             (const char* format, char_t* data, uint32_t length, uint32_t struct_size, uint32_t struct_offset);
    ubyte_t		get_background_color		();
    void		set_bc                      (uchar_t background_color);
    /**
     * @brief Overrides the background of the whole screen.
     * @param background_color
     */
    void		set_bc_all                  (uchar_t background_color);
    void		set_fc                      (uchar_t foreground_color);
    /**
     * @brief Overrides the foreground of the whole screen.
     * @param foreground_color
     */
    void		set_fc_all                  (uchar_t foreground_color);
    void        set_color_reset             ();
    void        set_color                   (uchar_t color);
    void        set_color_all               (uchar_t color);
    void		write_f                     (const char* args, ...);
    void		set_cursor                  (uint16_t row, uint16_t col);
    void		get_cursor                  (uint16_t* row, uint16_t* col);
    void		new_line					();
    void        tm_tab                      ();
    void        tm_backspace                ();
}}
