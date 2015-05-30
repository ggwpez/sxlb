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
    /*int c;
    int tmp[5];

    for (c = 0; c < 5; ++c)
        tmp[c] = memory::k_malloc(10, 0, 0);

    //for (c = 0; c < 10; ++c)
        //memory::k_free(tmp[0]);*/

    printlf("pid: 11111");

    task::create(two);
    task::end();
};

void three();
void two()
{
    printlf("pid: 22222");

    task::create(three);
    task::end();
};

void three()
{
    printlf("pid: 33333");

    task::create(one);
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
    ui::text::init(80, 50, FC_GREEN | BC_BLACK);
#endif

    printlf("done");
    //user::start();
    time::install();

    task::create(one);
    task::multitasking_set_enabled(true);

	return 0;
};
