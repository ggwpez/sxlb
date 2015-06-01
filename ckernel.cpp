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
    printlf("pid: %u", task::get_active_tasks());

    task::create(two);

    //while(1);
    task::end();
};

void three();
void two()
{
    printlf("pid: %u", task::get_active_tasks());

    task::create(three);
    //while(1);
    task::end();
};

void three()
{
    printlf("pid: %u", task::get_active_tasks());

    task::create(one);
    //while(1);
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
#else
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);
#endif

    //printlf("done");
    //user::start();
    time::install();

    task::create(one);

    task::multitasking_set_enabled(true);

    while(2) printlf("hi");
	return 0;
};
