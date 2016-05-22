#include "textmode.hpp"

namespace2(ui, text)
{
    byte_t* vram = 0xb8000;
    bool initialized = false;
    #define CHECK_INIT if (!initialized) return 0;
    #define VIDEO tm.font
    #define TEXT !tm.font
    #define PUT_C(c) (((uint16_t*)vram)[(tm.col + tm.row *tm.columns)] = (tm.t_clr << 8) | (c & 0xff))

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
    text tm;

    clr32_t save_fc, save_bc;
    clr44_t save_clr;

    void init(video::video_init_t* vdata, clr32_t fc_color, clr32_t bc_color, Font::Font_info* font)
    {
        tm.columns = vdata->w / font->distW;
        tm.rows    = vdata->h / font->distH;
        tm.col = 0;
        tm.row = 0;
        tm.tab_size = 4;
        tm.v_fc = fc_color;
        tm.v_bc = bc_color;
        save_fc = fc_color;
        save_bc = bc_color;
        tm.font = font;
        tm.vdata = *vdata;
        tm.pitch = vdata->w *tm.font->distH *vdata->bypp;          //bytes per row
        vram = vdata->fb;
        initialized = true;
        clear_screen();
        //update();
    };

    void init(uint16_t cols, uint16_t rows, clr44_t color)
    {
        tm.columns = cols;
        tm.rows = rows;
        tm.col = 0;
        tm.row = 0;
        tm.tab_size = 4;
        tm.t_clr = color;
        save_clr = color;
        tm.font = nullptr;
        tm.font = nullptr;

        initialized = true;
        set_fc(tm.t_clr);
        set_bc(tm.t_clr);
        clear_screen();
        update();
    };

    void scroll_down()
    {
        if (VIDEO)
        {
            for (uint32_t i = 0; i < tm.rows *tm.pitch; i += tm.pitch)
            {
                uint8_t* start = vram +i;
                uint8_t* end = vram +i +tm.pitch;

                memory::memcpy(start, end, tm.pitch);
            }

            video::draw_rect_filled(0, (tm.rows -1) *tm.font->distH, tm.vdata.w, tm.font->distH, tm.v_bc);
            set_cursor(0, tm.rows -1);
        }
        else
        {
            for (int i = 1; i < tm.rows; i++)
                memory::memcpy(vram + (((i -1)* tm.columns) << 1), vram + ((i* tm.columns) << 1), tm.columns << 1);

            for (int i = 0; i < tm.columns << 1; i += 2)
                *(vram + i +(((tm.rows -1)* tm.columns) << 1)) = ' ';
        }
    }

    void tm_line_feed()
    {
        CHECK_INIT

        tm.row++;
        update();
    };

    void tm_carriage_return()
    {
        CHECK_INIT

        tm.col = 0;
        update();
    };

    void tm_tab()
    {
        CHECK_INIT

        if(!tm.tab_size)
            return;

        uint16_t num = tm.tab_size -(tm.col %tm.tab_size);
        while(num--)
            put_char(' ');

        update();
    };

    void tm_backspace()
    {
        CHECK_INIT

        if (tm.col == 0)
        {
            if (tm.row == 0)
            { }	//already pSosition 0/0, nothing to do here
            else
            {
                tm.row--;
                tm.col = tm.columns - 1;

                if (VIDEO)
                    video::draw_rect_filled(tm.col * tm.font->distW, tm.row * tm.font->distH, tm.font->W, tm.font->H, tm.v_bc);
                else
                    PUT_C(0);
            }
        }
        else
        {
            tm.col--;

            if (VIDEO)
                video::draw_rect_filled(tm.col * tm.font->distW, tm.row * tm.font->distH, tm.font->distW, tm.font->distH, tm.v_bc);
            else
                PUT_C(0);
        }
        update();
    };

    void sxlb_textmoxe_set_cursor(uint16_t row, uint16_t col)
    {
        CHECK_INIT

        tm.row = row;
        tm.col = col;
    };

    void clear_screen()
    {
        CHECK_INIT

        if (VIDEO)
            video::fill_s(tm.v_bc);
        else
        {
            int32_t i = ((tm.columns*tm.rows) << 1) - 1;
            while (i > 0)
            {
                vram[i--] = tm.t_clr;
                vram[i--] = ' ';
            }
        }
        set_cursor(0, 0);
    };

    clr44_t get_background_color()
    {
        CHECK_INIT

        return tm.t_clr;
    };

    void dump_alphabet()
    {
        CHECK_INIT

        uint8_t i = 32;

        while (++i < 128)
            put_char(i);

        put_char('\n');
    };

    void new_line()
    {
        CHECK_INIT

        tm_carriage_return();
        tm_line_feed();
    };

    uint32_t write_in_line(const char* message, unsigned int line)
    {
        CHECK_INIT

        set_cursor(0, line);
        write(message);
    };

    void set_color_reset()
    {
        if (VIDEO)
        {
            vm_set_bc(save_bc);
            vm_set_fc(save_fc);
        }
        else
        {
            tm_set_bc(save_clr);
            tm_set_fc(save_clr);
        }
    }

    void set_bc(clr32_t bc)
    {
        if (VIDEO)
            vm_set_bc(bc);
        else
            tm_set_bc(CLR_32_8(bc));
    }

    void vm_set_bc(clr32_t bc)
    {
        tm.v_bc = bc;
    }

    void tm_set_bc(clr44_t bc)
    {
        bc &= B(11110000);

        tm.t_clr &= B(00001111);
        tm.t_clr |= bc;
    }

    void set_fc(clr32_t fc)
    {
        if (VIDEO)
            vm_set_fc(fc);
        else
            tm_set_fc(CLR_32_4(fc));
    }

    void vm_set_fc(clr32_t fc)
    {
        tm.v_fc = fc;
    }

    void tm_set_fc(clr4_t fc)
    {
        fc &= B(00001111);

        tm.t_clr &= B(11110000);
        tm.t_clr |= fc;
    }

    /*printl*/
    void write_line(const char* text)
    {
        CHECK_INIT

    #if __CHECKS_NLPTR
        if (!text)
            return;
    #endif

        write(text);

        tm.col = 0;
        tm.row++;
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

        tm.col = 0;
        tm.row++;
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
                char buffer[21];
                switch (args[++i])
                {
                    case 'l':
                    {
                        uint64_t ul = va_arg(ap, uint64_t);
                        sxlb_convert_itoa_u64_dec(ul, buffer);
                        write(buffer);

                    } break;
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
                        put_char(args[i]);
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

    void put_char(char c, clr44_t color)
    {
        CHECK_INIT

        ubyte_t tmp = tm.t_clr;
        tm.t_clr = color;

        put_char(c);
        tm.t_clr = tmp;
    };

    void put_char(char c)
    {
        CHECK_INIT

       // if (TEXT)
            //vram[((text_mode.col + text_mode.row*text_mode.columns) << 1) -1] = text_mode.text_color;   //set the text color, should put this in PUT_C but cant get it working with (uint16_t*)vram ...

        if (c >= 0 && c < 32 || c == 127)       //it is a controll char?
        {
            switch(c)
            {
                case '\n':
                {
                    tm_carriage_return();
                    tm_line_feed();
                } break;
                case '\t':
                {
                    tm_tab();
                } break;
                case '\b':
                {
                    tm_backspace();
                } break;
                case '\f':
                {
                    clear_screen();
                } break;
                default:
                {
                    PUT_C('?');

                    tm.col++;
                    update();
                } break;
            }
        }
        else if (c == ' ')
        {
            /*if (VIDEO)		//overdraw the space
                video::draw_rect_filled(tm.col * tm.font->distW, tm.row * tm.font->distH, tm.font->distW, tm.font->distH, tm.v_bc);
            else*/
                PUT_C(0);

            tm.col++;
            update();
        }
        else
        {
            if (VIDEO)      //write to the buffer of the video mode
                video::draw_char(tm.col *tm.font->distW, tm.row *tm.font->distH, tm.font, tm.v_fc, c - 33);
            else
                PUT_C(c);

            tm.col++;
            update();
        }
    };

    void get_size(uint16_t& cols, uint16_t& rows)
    {
        CHECK_INIT

        cols = tm.columns;
        rows = tm.rows;
    };

    void set_tab_with(uint8_t columns)
    {
        CHECK_INIT

        tm.tab_size = columns;
    };

    uint8_t get_tab_with()
    {
        CHECK_INIT
        return tm.tab_size;
    };

    uint16_t old_pos = 0xffff;
    void update()
    {
        CHECK_INIT

        while(tm.col >= tm.columns)
        {
            tm.col-=tm.columns;
            tm.row++;
        }
        if (tm.col >= tm.columns)
            tm.col = 0;
        if(tm.row >= tm.rows)
        {
            scroll_down();
            tm.row = tm.rows -1;
        }

        if (TEXT)
        {
            uint16_t pos = tm.col + tm.row *tm.columns;

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

        tm.col = col;
        tm.row = row;
        update();
    }

    void get_cursor(uint16_t* col, uint16_t* row)
    {
        CHECK_INIT

        *col = tm.col;
        *row = tm.row;
    }
}}
