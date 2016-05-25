#include "textmode.hpp"

namespace2(ui, text)
{
    #define CHECK_INIT if (!initialized) return 0;

    uint32_t write_in_line(const char* message, unsigned int line)
    {
        CHECK_INIT

        set_cursor(0, line);
        write(message);
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
}}
