#pragma once

#include "../types.hpp"
#include "../sprintf.hpp"
#include "../system/cpu_port.hpp"
#include "../convert.hpp"
#include "../ui/video.hpp"

#define IO
#define LOG

#ifdef IO
#define print 			(ui::text::write)
#define printl 			(ui::text::write_line)
#define printf          (ui::text::write_f)
#define printfl 		(ui::text::write_line_f)
#define putc 			(ui::text::put_char)
#define vprintf 		(ui::text::v_write_f)
#define printf_arr32 	(ui::text::write_f_array32)
#else
#define print
#define printl
#define printf
#define printfl
#define putc
#define vprintf
#define printf_arr32
#endif

namespace2 (time, pic)
{
    extern uint32_t get_ms();
}}

#ifdef LOG
#define logt ui::text::write_f("[ %u ms ] ", time::pic::get_ms()); ui::text::write_f
#define log  ui::text::write_f

#define logtINF         ui::text::set_color_reset(); logt
#define logINF          ui::text::set_color_reset(); log
#define logDBG          ui::text::set_fc(CLR::BLUE); logt
#define logOK           ui::text::set_fc(CLR::GREEN); ui::text::write(" [ ok ]");
#define logWAR          ui::text::set_fc(CLR::RED); logt
#define logERR          ui::text::set_fc(CLR::ORANGE); logt
#define logDONE         logOK; ui::text::new_line(); ui::text::set_color_reset();
#else
#define logtINF
#define logINF
#define logDBG
#define logOK
#define logWAR
#define logERR
#define logDONE
#endif

enum CLR : clr32_t
{
    RED = 0xff << 16,
    ORANGE = 0xff << 16 | 0x8c << 8,
    GREEN = 0xff << 8,
    BLUE = 0xff
};

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

#define CHECK_INIT if (!initialized) return 0;
#define VIDEO tm.font
#define TEXT !tm.font
#define PUT_C(c) (((uint16_t*)zbuff)[(tm.col + tm.row *tm.columns)] = (tm.t_clr << 8) | (c & 0xff))

namespace2(ui, text)
{
    struct text
    {
        uint16_t columns, rows;
        uint16_t row, col;
        uint8_t tab_size;
        clr32_t v_fc, v_bc;
        ubyte_t t_clr;
        uint32_t pitch;
        video::video_init_t vdata;
        Font::Font_info* font;
    }__attribute__((packed));

    extern text tm;
    extern byte_t* zbuff;
    extern byte_t* vram;
    extern bool initialized;

    void		write						(const char_t* text);
    void		update                      ();
    void        sync                        ();
    void		put_char					(char c);
    void		put_char					(char c, clr44_t color);
    void 		get_size                    (uint16_t& cols, uint16_t& rows);
    void		writeline					(const char_t* text);
    void		v_write_f					(const char* args, va_list ap);
    void		set_cursor                  (uint16_t col, uint16_t row);
    uint8_t		get_tab_with				();
    void		set_tab_with				(uint8_t rows);
    void		clear_screen				();
    void		init						(video::video_init_t* vdata, clr32_t fc_color, clr32_t bc_color, Font::Font_info* font);
    void		init						(uint16_t cols, uint16_t rows, clr44_t color);
    void        write_line                  (const char* text);
    void		write_line_f				(const char* args, ...);
    uint32_t	write_in_line				(const char* message, unsigned int line);
    void		dump_alphabet				();
    /*e.g.: printf_arr32("%u ", array, array.size, 10, 2). The last parameter can be used to print only the ints from an struct array, then you just give the offset from the base of the struct to the needed int and the size of the struct.*/
    ubyte_t		get_background_color		();
    /**
     * @brief Overrides the background of the whole screen.
     * @param background_color
     */
    void        vm_set_bc                   (clr32_t bc);
    void        tm_set_bc                   (clr8_t bc);
    void        vm_set_fc                   (clr32_t fc);
    void        tm_set_fc                   (clr4_t fc);
    void		set_fc                      (uint32_t fc);
    void        set_bc                      (uint32_t bc);
    void		write_f                     (const char* args, ...);
    void		set_cursor                  (uint16_t row, uint16_t col);
    void		get_cursor                  (uint16_t* row, uint16_t* col);
    void        set_color_reset             ();
    void		new_line					();
    void        tm_tab                      ();
    void        tm_backspace                ();
    void        tm_line_feed                ();
    void        tm_carriage_return          ();
}}
