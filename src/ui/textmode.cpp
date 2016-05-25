#include "textmode.hpp"

//#define BUFFERED

namespace2(ui, text)
{
    byte_t* vram = 0xb8000;
    byte_t* zbuff = nullptr;
    bool initialized = false;
    text tm;

    int vsync = 0;
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
        tm.pitch = vdata->w *tm.font->distH *vdata->bypp;          //bytes per text-row
        vram = vdata->fb;

#ifdef BUFFERED
        zbuff = memory::k_malloc(vdata->len, 0, NULL);
        vdata->fb = zbuff;
        video::init(*vdata);
        vdata->fb = vram;
#else
        zbuff = vram;
        video::init(*vdata);
#endif

        initialized = true;
        clear_screen();
        update();
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

        zbuff = vram;                                               //textmode is unbuffered
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
            memory::memcpy(zbuff, zbuff +tm.pitch, tm.pitch *(tm.rows -1));

            video::draw_rect_filled(0, (tm.rows -1) *tm.font->distH, tm.vdata.w, tm.font->distH, tm.v_bc);
        }
        else
        {
            memory::memcpy(zbuff, zbuff +(tm.columns << 1), (tm.columns *(tm.rows -1)) << 1);

            for (int i = 0; i < tm.columns << 1; i += 2)
                *(zbuff + i +(((tm.rows -1)* tm.columns) << 1)) = ' ';
        }
    }

    void tm_line_feed()
    {
        CHECK_INIT

        tm.row++;
        update();
#ifdef BUFFERED
        sync();
#endif
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
                zbuff[i--] = tm.t_clr;
                zbuff[i--] = ' ';
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

    void sync()
    {
        if (VIDEO)
        {
            if (vsync)
            {
                while ((io::asm_inb(0x03da) & 0x08));
                while (!(io::asm_inb(0x03da) & 0x08));
            }

            memory::memcpy(vram, zbuff, tm.vdata.len);

            /*
             * This looks spooky:
             *
             * for (int i = 0; i < tm.vdata.h; i++)
             *     memory::memcpy(vram +i*tm.vdata.w, zbuff +i*tm.vdata.pitch, tm.vdata.w *tm.vdata.bypp);
             * */
        }
    }

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
