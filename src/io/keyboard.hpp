#pragma once
#include "../types.hpp"
#include "../utils/queue.hpp"

#define getc (io::keyboard::get_char())
namespace2(io, keyboard)
{
    enum Keys : unsigned char
    {
        ESC = 27,
        F1  = 0x80,
        F2  = (F1 + 1),
        F3  = (F2 + 1),
        F4  = (F3 + 1),
        F5  = (F4 + 1),
        F6  = (F5 + 1),
        F7  = (F6 + 1),
        F8  = (F7 + 1),
        F9  = (F8 + 1),
        F10 = (F9 + 1),
        F11 = (F10 + 1),
        F12 = (F11 + 1),

        INS = 0x90,
        DEL = (INS + 1),
        POS1 = (DEL + 1),
        END = (POS1 + 1),
        PGUP = (END + 1),
        PGDN = (PGUP + 1),
        LEFT = (PGDN + 1),
        UP = (LEFT + 1),
        DOWN = (UP + 1),
        RIGHT = (DOWN + 1),

        RETURN = 26,
        PRINT = (RETURN + 1),
        PAUSE = (PRINT + 1),
        LWIN = (PAUSE + 1),
        RWIN = (LWIN + 1),
        MENU = (RWIN + 1),

        RLEFT_CTRL = 0x1D,
        RRIGHT_CTRL = 0x1D,

        RLEFT_ALT = 0x38,
        RRIGHT_ALT = 0x38,

        RLEFT_SHIFT = 0x2A,
        RRIGHT_SHIFT = 0x36,

        RCAPS_LOCK = 0x3A,
        RSCROLL_LOCK = 0x46,
        RNUM_LOCK = 0x45,
        RDEL = 0x53
    };

    #define ESC         1
    #define BACKSPACE   '\b'
    #define TAB         '\t'
    #define ENTER       '\n'
    #define RETURN      '\r'
    #define NEWLINE     ENTER

    #define META_ALT    B(00000001)
    #define META_CTRL   B(00000010)
    #define META_SHIFT  B(00000100)
    #define META_ANY    (META_ALT | META_CTRL | META_SHIFT)
    #define META_CAPS   B(00001000)
    #define META_NUM    B(00010000)
    #define META_SCRL   B(00100000)

    #define KEY_CODE(k)    ( k & 0xff)
    #define KEY_PRESSED(k) ((k & 0xff00))
    #define KEY_METAS(k)   ((k & 0xff0000) >> 16)
    //| Reserved|META_KEYS|IsPressed| Scancode|
    //|0000 0000|0000 0000|0000 0000|0000 0000|
    typedef unsigned int key_state_t; 

    typedef ::queue<key_state_t, 64> key_queue_t;

    static const uchar_t asciiNonShift[128] = {
        0, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
        TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, 0, 0,
        POS1, UP, PGUP, '-', LEFT, '5', RIGHT, '+', END, DOWN, PGDN, INS, DEL, 0, 0, 0, F11, F12 };

    static const uchar_t asciiShift[128] = {
        0, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', BACKSPACE,
        TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, 0,
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, 0, 0,
        POS1, UP, PGUP, '-', LEFT, '5', RIGHT, '+', END, DOWN, PGDN, INS, DEL, 0, 0, 0, F11, F12 };

    void init();
    void flush();
    key_state_t get_key();
    uchar_t get_char();
    uchar_t state_to_char(key_state_t state);
}}
