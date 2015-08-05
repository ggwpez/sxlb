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

#define VIDEO_MODE 1

void two();
void one()
{
    printlf("%u ", task::get_pid());

    if (!task::create(two));
    task::end();
};

void three();
void two()
{
    printlf("%u", task::get_pid());

    if (!task::create(three));
    task::end();
};

void three()
{
    printlf("%u", task::get_pid());

    if (!task::create(one)) syshlt("33");
    task::end();
};

int32_t main()
{
    sxlb_gdt_load();
    idt::load();
    sti

    memory::init();
    //finit
#if VIDEO_MODE == 1
    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, true);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::update();
#else
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);
#endif


    hw::keyboard::init();
    while (1)
    {
	ui::text::put_char(hw::keyboard::getc());
   	ui::video::update();
    }
    //user::start();



    //task::create(one);

    //task::multitasking_set_enabled(true);

    while(1);
    return 0;
};
