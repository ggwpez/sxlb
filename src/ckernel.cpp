#include "ui/textmode.hpp"
#include "ui/video.hpp"
#include "idt/idt.hpp"
#include "types.hpp"
#include "time/timer.hpp"
#include "convert.hpp"

#include "gdt.hpp"
#include "memory/memory.hpp"
#include "sprintf.hpp"
#include "font.hpp"
#include "hw/keyboard.hpp"
#include "user/test.hpp"
#include "system/syscall.hpp"

#define VIDEO_MODE 0

volatile uint32_t v;
void two();
void one()
{
    task::create2(two);

    task::end();
};

void three();
void two()
{
    task::create2(three);

    task::end();
};

void three()
{
    task::create2(one);

    task::end();
};

void root()
{
    memory::dump_info(nullptr);

    stop
}

int32_t main()
{
    sxlb_gdt_load();
    idt::load();
    time::install();

    //finit
#if VIDEO_MODE == 1
    memory::init();

    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, true);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::update();
#else
    memory::init();
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);    //here i can already init textmode, so i see errors from memory::init, maybe do it in VIDEO_MODE as well?
#endif 
    //time::set_frequenze(1);
    //system::init();

    printl("Kernel loaded.");
    task::create2(one);

    task::multitasking_set_enabled(true);
    TASK_SWITCH
    ui::video::update();
    stop
    return 0;
};
