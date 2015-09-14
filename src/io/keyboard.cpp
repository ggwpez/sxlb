#include "keyboard.hpp"
#include "hwaccess.hpp"
#include "../task/task.hpp"
#include "../idt/idt.hpp"

namespace2(io, keyboard)
{
    void keyboard_interrupt_handler(task::cpu_state_t* state);
    void mouse_interrupt_handler   (task::cpu_state_t* state);
    bool keys_pressed[256] = { false };

    void init()
    {
        flush();
        memory::memset(keys_pressed, 0, sizeof(keys_pressed));
        idt::irq_register_event_handler(1, keyboard_interrupt_handler);
        //idt::irq_register_event_handler(4, mouse_interrupt_handler);
    }

    void keyboard_interrupt_handler(task::cpu_state_t* state)
    {
        uint8_t scan = io::asm_inb(0x60);

        if (scan)
            printfl("k: %x ", scan);
    }

    void mouse_interrupt_handler(task::cpu_state_t* state)
    {
        printl("mouse!");
    }

    void flush()
    {
        //clear buffer
        while (io::asm_inb(0x64) & 1)
            io::asm_inb(0x60);
    }

    key_state_t get_key()
    {
        while (!(io::asm_inb(0x64) & 1));
        uint8_t code = io::asm_inb(0x60), key = code & B(01111111);
        bool pressed = !(code & B(10000000));               //sing bit indicates if the key was pressed or released

        uchar_t port_value = io::asm_inb(0x61);	//Let the keyboard know, that we got it
        io::asm_outb(0x61, port_value | 0x80);	// 0->1
        io::asm_outb(0x61, port_value &~0x80);	// 1->0

        if (code == 0xe0 || code == 0xe1 || code == 0xe2)
            return 0;

        if (key == Keys::RCAPS_LOCK || key == Keys::RNUM_LOCK || key == Keys::RSCROLL_LOCK)
        {
            if (pressed)                                    //toggle only on pressed
                keys_pressed[Keys::RCAPS_LOCK] ^= true;

            return 0;
        }
        else if (key == Keys::RRIGHT_SHIFT ||
                 key == Keys::RLEFT_SHIFT ||
                 key == Keys::RLEFT_CTRL ||
                 key == Keys::RLEFT_ALT)                    //toggle also on release
        {
            keys_pressed[key] ^= true;
            return 0;
        }
        else
            keys_pressed[key] ^= true;

        ubyte_t metas;                              //set meta keys, for key_state_t
        metas = keys_pressed[RLEFT_ALT]     << 0 |
                keys_pressed[RLEFT_CTRL]    << 1 |
                (keys_pressed[RLEFT_SHIFT] | keys_pressed[RRIGHT_SHIFT])   << 2 |
                keys_pressed[RCAPS_LOCK]    << 3 |
                keys_pressed[RNUM_LOCK]     << 4 |
                keys_pressed[RSCROLL_LOCK]  << 5;

        return key_state_t(metas << 16 | (pressed ? 0xff : 0) << 8 | code);
    }

    uchar_t state_to_char(key_state_t state)
    {
        //wont check for is_pressed
        ubyte_t metas = KEY_METAS(state);

        if ((META_CAPS & metas) ^ (META_SHIFT & metas))
            return asciiShift[KEY_CODE(state)];
        else
            return asciiNonShift[KEY_CODE(state)];
    }
}}
