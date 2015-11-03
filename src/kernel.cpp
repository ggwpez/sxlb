#include "ui/textmode.hpp"
#include "ui/window.hpp"
#include "ui/video.hpp"
#include "idt/idt.hpp"
#include "types.hpp"
#include "time/timer.hpp"
#include "convert.hpp"

#include "fs/initrd.hpp"
#include "gdt.hpp"
#include "memory/memory.hpp"
#include "sprintf.hpp"
#include "font.hpp"
#include "io/keyboard.hpp"
#include "user/test.hpp"
#include "system/syscall.hpp"
#include "user/elf.hpp"
#include "user/executable.hpp"

#define VIDEO_MODE 0
#define LOGGING 1

extern "C"
{
    extern void data_start();
    extern void data_end  ();
}

void init()
{
    ui::text::init(80, 25, FC_LIGHTGRAY | BC_BLACK);

    logINF("gdt:\n");
    gdt::init();
    logINF("idt:\n");
    idt::load();
    sti
    logINF("vmm:\n");
    memory::init();
    logINF("sys:\n");
    system::init();
    logINF("pic:\n");
    time::init();
    logINF("keyboard:\n");
    io::keyboard::init();
    logINF("tasking:\n");
    task::init();
    logINF("initrd:\n");
    vfs::fs_node_t* initrd = initrd::fs_install(&data_start);
    logINF("vfs:\n");
    vfs::init(initrd);
    logINF("window manager:\n");
    ui::window::init();

    for (int i = 0; i < 80; ++i) { logINF("="); }
    logINF("Kernel loaded. Press any key to continue.");

    ui::text::clear_screen();
}

bool running = true;
void idle();
void shut_down();
int32_t main()
{
    finit;
    init();
    char* argv[] = { "/initrd/bash.dat", nullptr };
    execve(nullptr, argv[0], argv, nullptr);

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
/*#if VIDEO_MODE == 1
    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, false);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::clear_screen(ui::video::VC_DARKGRAY);
    ui::video::update();
#else

#endif*/
