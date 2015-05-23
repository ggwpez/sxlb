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

#define VIDEO_MODE 0

void one()
{
	while (true)
		printf("%b", task::get_pid());
		//sxlb_text_putchar(hw::keyboard::getc());
}

void two()
{
	while (true)
		printf("%b", task::get_pid());
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
	ui::video::init(320, 200, video::VC_DARKGRAY, zBuffer, true);
	ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
#else
	ui::text::init(80, 50, FC_GREEN | BC_BLACK);
#endif

	//video::draw_line(10,15, 150, 60, video::video_color::VC_BLUE);
	
	hw::keyboard::init();
	
	int arr[2000];
	for (int i = 0; i < 15; i++)
		if ((arr[i] = k_malloc(1024*1024)) == nullptr)
			{ printlf("error"); hlt }
	
	sxlb_memory_dump_info(nullptr);
	
	/*for (int i = 0; i < 1500; i++)
		if (!k_free(arr[i]))
			printlf("error");
	
	sxlb_memory_dump_info(nullptr);*/
	//test::run();
	/*sxlb_timer_install();
	sti

	task::task_create(one);
	task::task_create(two);
	task::task_create(update);

	task::multitasking_set_enabled(true);*/
	
	//printf("\n\nEnd");
	ui::video::update();
	hlt
	return 0;
};
