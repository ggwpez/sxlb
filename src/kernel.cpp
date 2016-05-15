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
    ui::video::video_init_t data;
    ui::text::init(80, 25, FC_LIGHTGRAY | BC_BLACK);
    logtINF("boot:\n");
    mb::init(mbi, magic, &data);

    ui::video::init(&data);
    ui::text::init(800, 600, 0xff << 8, 0, &Font::Lucidia_Console);
    ui::text::clear_screen();

    mb::init(mbi, magic, &data);

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

    io::keyboard::get_char();
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

int32_t main(void*, uint32_t);
void test()
{
    uint8_t* ptr = 0xa0000;
    uint32_t i = 0;

    while (ptr < 0xbffff)
        *ptr++ = (uint8_t)ptr % 256,
        *ptr++ = (uint8_t)ptr % 256,
        *ptr++ = 0;

    hlt
}

bool running = true;
void idle();
void shut_down();
int32_t main(void* ptr, uint32_t magic)
{
    //test();

    finit;
    init(ptr, magic);
    hlt

#define s 10
    uint32_t mems[s];
    for (int i = 0; i < s; i++)
        mems[i] = memory::k_malloc(100, 0, 0);
    for (int i = 0; i < s; i++)
        memory::k_free(mems[i]);


    char* argv[] = { "/initrd/nasm.dat", /*"cat initrd/nasm.dat",*/ nullptr };
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
/*#if VIDEO_MODE == 1
    LPTR zBuffer = memory::k_malloc(64000, 0, nullptr);
    ui::video::init(320, 200, ui::video::VC_DARKGRAY, zBuffer, false);
    ui::text::init(320, 200, FC_GREEN, &Font::Lucidia_Console);
    ui::video::clear_screen(ui::video::VC_DARKGRAY);
    ui::video::update();
#else

#endif*/
