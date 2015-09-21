#include "textmode.hpp"

namespace2(ui, text)
{
    byte_t* vram = 0xb8000;
    bool initialized = false;
    #define CHECK_INIT if (!initialized) return 0;
    #define VIDEO text_mode.font
    #define TEXT !text_mode.font
    #define PUT_C(c) (((uint16_t*)vram)[(text_mode.col + text_mode.row*text_mode.columns)] = text_mode.text_color << 8 | (c & 0xff))
    ubyte_t def_color;

    struct text
    {
        uint16_t columns, rows;
        uint16_t row, col;
        uint8_t tab_size;
        ubyte_t text_color;
        Font::Font_info* font;
    }__attribute__((packed));
    text text_mode;

    void init(uint16_t pixelW, uint16_t pixelH, ubyte_t default_color, Font::Font_info* font)
    {
        text_mode.columns = pixelW / font->distW;
        text_mode.rows    = pixelH / font->distH;
        text_mode.col = 0;
        text_mode.row = 0;
        text_mode.tab_size = 4;
        def_color = default_color;
        text_mode.text_color = def_color;
        text_mode.font = font;
        initialized = true;
        update();
    };

    void init(uint16_t cols, uint16_t rows, ubyte_t default_color)
    {
        text_mode.columns = cols;
        text_mode.rows = rows;
        text_mode.col = 0;
        text_mode.row = 0;
        text_mode.tab_size = 4;
        def_color = default_color;
        text_mode.text_color = default_color;
        text_mode.font = nullptr;

        initialized = true;
        clear_screen();
        set_fc_all(default_color);
        set_bc_all(default_color);
        update();
    };

    void scroll_down()
    {
        for (int i = 1; i < text_mode.rows; i++)
            memory::memcpy(vram + (((i-1)* text_mode.columns) << 1), vram + ((i* text_mode.columns)<<1), text_mode.columns << 1);

        for (int i = 0; i < text_mode.columns << 1; i += 2)
            *(vram + i +(((text_mode.rows -1)* text_mode.columns) << 1)) = ' ';
    }

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

        update();
    };

    void tm_backspace()
    {
        CHECK_INIT

        if (text_mode.col == 0)
        {
            if (text_mode.row == 0)
            { }	//already pSosition 0/0, nothing to do here
            else
            {
                text_mode.row--;
                text_mode.col = text_mode.columns - 1;

                if (VIDEO)
                    video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);
                else
                    PUT_C(0);
            }
        }
        else
        {
            text_mode.col--;

            if (VIDEO)
                video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);
            else
                PUT_C(0);
        }
        update();
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

        if (VIDEO)
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

    void set_color_reset()
    {
        set_color(def_color);
    }

    void set_color(uchar_t color)
    {
        text_mode.text_color = color;
    }

    void set_color_all(uchar_t color)
    {
        CHECK_INIT

        uint32_t i = 0, end = (text_mode.rows * text_mode.columns) << 1;

        while (i != end)
        {
            vram[++i] == B(00001111);
            ++i;
        }

        text_mode.text_color == color;
    }

    void set_bc(uchar_t bc)
    {
        bc &= B(11110000);

        text_mode.text_color &= B(00001111);
        text_mode.text_color |= bc;
    }

    void set_bc_all(uchar_t bc)
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

    void set_fc(uchar_t fc)
    {
        fc &= B(00001111);

        text_mode.text_color &= B(11110000);
        text_mode.text_color |= fc;
    }

    void set_fc_all(uchar_t fc)
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

    /*printl*/
    void write_line(const char* text)
    {
        CHECK_INIT

    #if __CHECKS_NLPTR
        if (!text)
            return;
    #endif

        write(text);

        text_mode.col = 0;
        text_mode.row++;
        update();
    };

    /*printfl*/
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
                case 'c':
                {
                    u = va_arg(ap, uint32_t);		//cant pass a byte, implicit dword
                    put_char(u & 0xff);
                } break;
                case 's':
                {
                    u = va_arg(ap, LPTR);			//cant pass a byte, implicit dword
                    write((char_t*)u);              //actuallisy is char_t*
                } break;
                case 'm':
                {
                    u = va_arg(ap, uint32_t);			//cant pass a byte, implicit dword

                    if ((u >> 30))
                    {
                        sxlb_convert_itoa_s16_dec((uint16_t)(u >> 30), buffer);
                        printf("%s GiB", buffer);
                    }
                    else if ((u >> 20))
                    {
                        sxlb_convert_itoa_s16_dec((uint16_t)(u >> 20), buffer);
                        printf("%s MiB", buffer);
                    }
                    else if ((u >> 10))
                    {
                        sxlb_convert_itoa_s16_dec((uint16_t)(u >> 10), buffer);
                        printf("%s KiB", buffer);
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

    void put_char(char c, ubyte_t color)
    {
        CHECK_INIT

        ubyte_t tmp = text_mode.text_color;
        text_mode.text_color = color;

        put_char(c);
        text_mode.text_color = tmp;
    };

    void put_char(char c)
    {
        CHECK_INIT

       // if (TEXT)
            //vram[((text_mode.col + text_mode.row*text_mode.columns) << 1) -1] = text_mode.text_color;   //set the text color, should put this in PUT_C but cant get it working with (uint16_t*)vram ...

        if (c)
        switch(c)
        {
            case ' ':
            {
                if (VIDEO)		//overdraw the space
                    video::draw_rect_filled(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font->distW, text_mode.font->distH, video::bg_color);
                else
                    PUT_C(0);

                text_mode.col++;
                update();
            } break;
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
            {
                if (VIDEO)      //write to the buffer of the video mode
                    video::draw_char(text_mode.col * text_mode.font->distW, text_mode.row * text_mode.font->distH, text_mode.font, text_mode.text_color, c - 33);
                else
                    PUT_C(c);

                text_mode.col++;
                update();
            } break;
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

    uint16_t old_pos = 0xffff;
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
        if(text_mode.row >= text_mode.rows)
        {
            scroll_down();
            text_mode.row = text_mode.rows -1;
        }

        if (TEXT)
        {
            uint16_t pos = text_mode.col + text_mode.row*text_mode.columns;

            if (pos != old_pos)
            {
                old_pos = pos;

                io::asm_outb(0x3D4, 0x0F);
                io::asm_outb(0x3D5, (ubyte_t)(pos & 0xFF));
                io::asm_outb(0x3D4, 0x0E);
                io::asm_outb(0x3D5, (ubyte_t)((pos >> 8) & 0xFF));
            }
        }
    };

    void set_cursor(uint16_t col, uint16_t row)
    {
        CHECK_INIT

        text_mode.col = col;
        text_mode.row = row;
        update();
    }

    void get_cursor(uint16_t* col, uint16_t* row)
    {
        CHECK_INIT

        *col = text_mode.col;
        *row = text_mode.row;
    }
}}
