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
    printlf("%u ", task::get_pid());

    if (!task::create(two)) syshlt("11");
    task::end();
};

void three();
void two()
{
    printlf("%u", task::get_pid());

    if (!task::create(three)) syshlt("22");
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
#else
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);
#endif

    //printlf("done");
    //user::start();
    time::install();


    task::create(one);

    task::multitasking_set_enabled(true);

    while(1);
	return 0;
};
