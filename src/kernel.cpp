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
#include "user/console.hpp"
#include "user/elf.hpp"

#define VIDEO_MODE 0
#define LOGGING 1

extern "C"
{
    extern void data_start();
    extern void data_end  ();
}

void one()
{
    console term;
    term.main();

    EXIT(0);
}

bool running = true;
void idle();
void shut_down();
int32_t main()
{
    ui::text::init(80, 25, FC_LIGHTGRAY | BC_BLACK);

    logINF("gdt:\n");
    gdt::init();
    logINF("idt:\n");
    idt::load();
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
    for (int i = 0; i < 80; ++i) { logINF("="); }
    printfl("Kernel loaded. Press any key to continue.");

    ui::text::clear_screen();


    LPTR initrd_start = &data_start;
    vfs::fs_node_t* initrd = initrd::fs_install(initrd_start);
    vfs::init(initrd);

    vfs::fs_node_t* initrd_dir = vfs::find_dir(&vfs::root_node, "initrd");
    vfs::fs_node_t* cdat = vfs::find_dir(initrd_dir, "c_test.dat");
    char buffer[cdat->length];
    vfs::read(cdat, 0, cdat->length, buffer);

    elf::elf_status_t st;
    LPTR* con = elf::load_file(buffer, &st);

    sti
    task::create(con, 0);

    /*uint16_t* x,* y; ui::text::get_cursor(x,y);
    putc(0xb3);print("Name"); ui::text::set_cursor(20, *y);
    putc(0xb3);print("Type"); ui::text::set_cursor(25, *y);
    putc(0xb3);print("Size"); ui::text::set_cursor(35, *y);
    putc(0xb3);printl("Content/{0-44}");

    for (int i = 0; i < vfs::root_node.length; ++i)
    {
        vfs::dir_ent_t* ent = vfs::read_dir(&vfs::root_node, i);
        vfs::fs_node_t* node = vfs::find_dir(&vfs::root_node, ent->name);
        ubyte_t data[45];

        if ((char)node->type & (char)vfs::node_type::Dir)
        {
            ui::text::get_cursor(x,y);

            ui::text::get_cursor(x,y);
            putc(0xb3); ui::text::set_fc(FC_TURQUOISE); printf("%s", node->name); ui::text::set_cursor(20, *y); ui::text::set_color_reset();
            putc(0xb3);print("DIR"); ui::text::set_cursor(25, *y);
            putc(0xb3); (node->length-1) == 1 ? printf("1 item") : printf("%u items", node->length-1); ui::text::set_cursor(35, *y);
            putc(0xb3);
            ui::text::new_line();
        }
        else
        {
            memory::memset(data, 0, sizeof(data));
            vfs::read(node, 0, sizeof(data), data);

            ui::text::get_cursor(x,y);
            putc(0xb3);printf(" %s", node->name); ui::text::set_cursor(20, *y);
            putc(0xb3);print("FILE"); ui::text::set_cursor(25, *y);
            putc(0xb3);printf("%m", node->length); ui::text::set_cursor(35, *y);
            putc(0xb3);
            for (int j = 0; j < sizeof(data); ++j)
                putc(data[j]);

            ui::text::new_line();
        }
    }*/

    /*LPTR mem = 0x400000;
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
    }*/

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