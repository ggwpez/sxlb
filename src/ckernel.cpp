#include "ui/textmode.hpp"
#include "ui/window.hpp"
#include "ui/video.hpp"
#include "idt/idt.hpp"
#include "types.hpp"
#include "time/timer.hpp"
#include "convert.hpp"

#include "fs/fs.hpp"
#include "gdt.hpp"
#include "memory/memory.hpp"
#include "sprintf.hpp"
#include "font.hpp"
#include "io/keyboard.hpp"
#include "user/test.hpp"
#include "system/syscall.hpp"
#include "user/console.hpp"

#define VIDEO_MODE 0

extern "C"
{
    extern void data_start();
    extern void data_end  ();
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
    time::init();

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
    //io::keyboard::init();
    sti
    //ui::window::init();

    size_t program_size = &data_end - &data_start;
    LPTR mem = 0x400000;
    memory::memcpy(mem, &data_start, program_size);

    fs_t* fs = fs_install(mem);

    fs_node_t* found = fs->find_file("test.o");
    if (found == nullptr)
        printfl("not found");
    else
    {
        printfl("found: %x", found);
        task::create(found->data, 0);
    }

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
