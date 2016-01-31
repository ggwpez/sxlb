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
#include "boot/multiboot.hpp"

#define VIDEO_MODE 0

extern "C"
{
    extern void data_start();
    extern void data_end  ();
}

void init(void* mbi, uint32_t magic)
{
    ui::text::init(80, 25, FC_LIGHTGRAY | BC_BLACK);
    logINF("boot:\n");
   /* mb::init(mbi, magic);

    uint32_t i = (uint32_t)-1;
    while (i--);*/

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
    //logINF("window manager:\n");
    //ui::window::init();

    for (int i = 0; i < 80; ++i) { logINF("="); }
    logINF("Kernel loaded. Press any key to continue.\n");

    ui::text::clear_screen();
    //io::keyboard::get_char();
}

void sig_test()
{
    uint32_t i = 4000000;
    while (i--);

    logOK("sending signal...");
    task::sig(2, 0x1e);  //SIGUSR1
    logDONE;
    stop
}

bool running = true;
void idle();
void shut_down();
int32_t main(void* mbi, uint32_t magic)
{
    finit;
    init(mbi, magic);
    char* argv[] = { "/initrd/bash.dat", /*"cat initrd/bash.dat",*/ nullptr };
    execve(nullptr, argv[0], argv, nullptr);
    task::create(&sig_test, 0,0, 0);

    task::multitasking_set(true);
    TASK_SWITCH

    printf("done\n");
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
