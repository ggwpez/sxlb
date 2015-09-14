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
#include "user/elf.hpp"

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
    sti;
    io::keyboard::init();

    /*task::init();
    //

    LPTR mem = 0x400000;
    memory::memcpy(mem, &data_start, &data_end - &data_start);

    fs_t* fs = fs_install(mem);

    fs_node_t* found = fs->find_file("c_test.dat");
    if (found == nullptr)
        printfl("not found");
    else
    {
        elf::elf_status_t s;
        LPTR entry = elf::load_file(found->data, &s);

        if (s != elf::elf_status_t::Ok)
            printfl("failed with %b @%x", s, entry);
        else
        {
            //printfl("type: %u\ncpu: %u\nversion: %u\nentry:%x\npht_off: %u\nsht_off: %u\nflags: %u\nsize: %u\npht_entry_s: %u\npht_entry_c: %u\nsht_entry_s: %u\nsht_entry_c: %u\nshstrndx: %u",
            //        h->type, h->cpu, h->version, h->entry, h->pht_off, h->sht_off, h->flags, h->this_size, h->pht_entry_s, h->pht_entry_c, h->sht_entry_s, h->sht_entry_c, h->shstrndx);

            task::create(entry, 3);
        }
    }

    task::multitasking_set(true);
    TASK_SWITCH*/

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
