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

void one()
{
	while (true)
		printf("%u", task::get_pid());
		//sxlb_text_putchar(hw::keyboard::getc());
}

void two()
{
	while (true)
		printf("%u", task::get_pid());
}

void update()
{
	while (1)
		ui::video::update();
}

int32_t main()
{
	sxlb_gdt_load();
	idt::load();
	
	sxlb_memory_init();
	cli
	finit
	
#if VIDEO_MODE == 1
	LPTR zBuffer = k_malloc(64000, 0, nullptr);
	ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, true);
	ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
#else
	ui::text::init(80, 50, FC_GREEN | BC_BLACK);
#endif

    /*Test for aligned allocation - the lazy way*/
    printf("cool");

    //user::start();
	/*time::install();
	sti
	
	task::task_create(one);
	task::task_create(two);
	task::task_create(update);

	task::multitasking_set_enabled(true);*/
	
	while (true);
	return 0;
};
