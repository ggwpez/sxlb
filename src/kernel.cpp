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
    finit

    ui::video::video_init_t data;
    logtINF("boot:\n");
    mb::init(mbi, magic, &data);

    if (data.type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB)
    {
        ui::video::init(&data);
        ui::text::init(data.w, data.h, 0xff << 8, 0, &Font::Lucidia_Console);
        ui::text::clear_screen();
        ui::video::fill_s(102 << 8 | 102);
    }
    else if (data.type == MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT)
    {
        ui::text::init(80, 25, FC_LIGHTGRAY | BC_BLACK);
    }

    mb::init(mbi, magic, &data);
stop
    logtINF("gdt:\n");
    gdt::init();
    logtINF("idt:\n");
    idt::load();
    sti
    logtINF("pic:\n");
    time::init();

    logtINF("vmm:\n");
    memory::init();
    logtINF("sys:\n");
    system::init();
    logtINF("keyboard:\n");
    io::keyboard::init();
    logtINF("tasking:\n");
    task::init();
    logtINF("initrd:\n");
    vfs::fs_node_t* initrd = initrd::fs_install(&data_start);
    logtINF("vfs:\n");
    vfs::init(initrd);
    //logtINF("window manager:\n");
    //ui::window::init();

    for (int i = 0; i < 80; ++i) { logINF("="); }
    logtINF("Kernel loaded. Press any key to continue.\n");
    //io::keyboard::get_char();
    ui::text::clear_screen();
}

void sig_test()
{
    uint32_t i = 4000000;
    while (i--);
    task::sig(2, 0x1f);  //SIGUSR1

    i = 4000000;
    while (i--);

    task::sig(2, 0x1e);  //SIGUSR2
    task::end(0);
}

bool running = true;
void idle();
void shut_down();
int32_t main(void* ptr, uint32_t magic)
{
    init(ptr, magic);

    #define s 10
    uint32_t mems[s];
    for (int i = 0; i < s; i++)
        mems[i] = memory::k_malloc(100, 0, 0);
    for (int i = 0; i < s; i++)
        memory::k_free(mems[i]);

    printfl("Test ended");
    stop

    char* argv[] = { "/initrd/bash.dat", /*"cat initrd/nasm.dat",*/ nullptr };
    printfl("Starting task:");
    execve(nullptr, argv[0], argv, nullptr);
    //task::create(&sig_test, 0,0, 0);


    task::multitasking_set(true);
    TASK_SWITCH

    printfl("done");
    idle();
    shut_down();
    return 0;
}

void idle()
{
    asm volatile("jmp .");
}

void shut_down()
{
    ui::text::clear_screen();
    printf("System halted.");
}
