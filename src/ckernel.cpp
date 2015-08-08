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

#define VIDEO_MODE 0

void two();
void one()
{
    if (!task::create(two)) syshlt("#1");
    task::end();
};

void three();
void two()
{
    if (!task::create(three)) syshlt("#2");
    task::end();
};

void three()
{
    if (!task::create(one)) syshlt("#3");
    task::end();
};

void root()
{
    while (1)
    {
        printfl("hier");
    }
}

int32_t main()
{
    sxlb_gdt_load();
    idt::load();
    sti

    //finit
#if VIDEO_MODE == 1
    memory::init();

    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, true);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::update();
#else
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);    //here i can already init textmode, so i see errors from memory::init, maybe do it in VIDEO_MODE as well?
    memory::init();
#endif 

    hw::keyboard::init();
    while (1)
    {
        ui::text::put_char(hw::keyboard::getc());
        //ui::video::update();
    }

    //task::create(one);
    //task::create(root);

    //task::multitasking_set_enabled(true);

    printl("is over");
    stop
    return 0;
};
