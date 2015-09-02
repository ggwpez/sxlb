#include "ui/textmode.hpp"
#include "ui/window.hpp"
#include "ui/video.hpp"
#include "idt/idt.hpp"
#include "types.hpp"
#include "time/timer.hpp"
#include "convert.hpp"

#include "gdt.hpp"
#include "memory/memory.hpp"
#include "sprintf.hpp"
#include "font.hpp"
#include "io/keyboard.hpp"
#include "user/test.hpp"
#include "system/syscall.hpp"
#include "user/console.hpp"

#define VIDEO_MODE 0

void two();
void one()
{
    console user_term;
    user_term.main();

    EXIT
}

void three();
void two()
{
    int i = 0;
    while(i++ < 10000)
        SYSCALL1(system::CALL::PUTC, 'a');

    EXIT
}

void three()
{
    int i = 0;
    while(i++ < 10000)
    {
        SYSCALL0(system::CALL::NOP);
    }

    EXIT
}

bool brk_handler(task::cpu_state_t* state, char* kill_msg)
{

}

bool running = true;
void idle();
void shut_down();
int32_t main()
{
    sxlb_gdt_load();
    idt::load();
    memory::init();
    system::init();
    time::install();  
    sti

#if VIDEO_MODE == 1
    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, false);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::clear_screen(ui::video::VC_DARKGRAY);
    ui::video::update();
#else
    ui::text::init(80, 25, FC_GREEN | BC_BLACK);
#endif
    task::init();
    //ui::window::init();

    task::create(one, 3);
    task::create(two, 3);
    task::create(two, 3);
    task::create(two, 3);
    task::create(two, 3);
    task::create(two, 3);
    task::create(two, 3);

    task::multitasking_set(true);
    TASK_SWITCH

    idle();
    shut_down();
    return 0;
}

void idle()
{
    while (running);
}

void shut_down()
{
    ui::text::clear_screen();
    printf("System halted.");
}
