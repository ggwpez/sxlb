#include "textmode.hpp"
#include "video.hpp"
#include "idt.hpp"
#include "types.hpp"
#include "timer.hpp"
#include "convert.hpp"

#include "gdt.hpp"
#include "memory.hpp"
#include "sprintf.hpp"
#include "font.hpp"
#include "keyboard.hpp"
#include "test.hpp"

#define VIDEO_MODE 0

void two();
void one()
{
    while (1)
    {
        printl("1");
    }

    /*if (!task::create(two)) syshlt("#1");
    task::end();*/
};

void three();
void two()
{
    while (1)
    {
        printl("2");
    }

    /*if (!task::create(three)) syshlt("#2");
    task::end();*/
};

void three()
{
    while (1)
    {
        printl("3");
    }

    /*if (!task::create(one)) syshlt("#3");
    task::end();*/
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
    memory::init();
    ui::text::init(80, 50, FC_GREEN | BC_BLACK);    //here i can already init textmode, so i see errors from memory::init, maybe do it in VIDEO_MODE as well?
#endif


    /*hw::keyboard::init();
    printf("hi");
    while (1)
    {
        ui::text::put_char(hw::keyboard::getc());
        ui::video::update();
    }*/

    //user::start();

    task::create(one);
    task::create(two);
    task::create(three);
    task::create(root);

    task::multitasking_set_enabled(true);

    while(1);
    return 0;
};
