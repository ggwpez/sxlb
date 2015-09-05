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
        idt::irq_register_event_handler(4, mouse_interrupt_handler);
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

    uint8_t get_key(bool& pressed)
    {
        while (!(io::asm_inb(0x64) & 1));
        uint8_t code = io::asm_inb(0x60), key = code & B(01111111);

        uchar_t port_value = io::asm_inb(0x61);	//Let the keyboard know, that we got it
        io::asm_outb(0x61, port_value | 0x80);	// 0->1
        io::asm_outb(0x61, port_value &~0x80);	// 1->0

        if (code == 0xe0 || code == 0xe1 || code == 0xe2)
        {
            pressed = false;
            return 0;
        }

        pressed = !(code & B(10000000));		//sing bit indicates if the key was pressed or released

        if (key == Keys::RCAPS_LOCK)
        {
            if (pressed)                                    //toggle only on pressed
                keys_pressed[Keys::RCAPS_LOCK] ^= true;

            return 0;
        }
        else if (key == Keys::RRIGHT_SHIFT || key == Keys::RLEFT_SHIFT) //toggle also on release
        {
            keys_pressed[key] ^= true;
            return 0;
        }
        else
            keys_pressed[key] ^= true;

        return key;
    }

    uchar_t getc()
    {
        bool was_pressed;
        uint8_t c = get_key(was_pressed);

        while (true)
        if (was_pressed && c != 0)
            break;
        else
            c = get_key(was_pressed);

        if (keys_pressed[Keys::RCAPS_LOCK] ^ (keys_pressed[Keys::RLEFT_SHIFT] || keys_pressed[Keys::RRIGHT_SHIFT]))
            return asciiShift[c];
        else
            return asciiNonShift[c];
    }
}}
