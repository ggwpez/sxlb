#include "multiboot.hpp"
#include "../ui/textmode.hpp"

#define CHECK(f, var, v, e) if ((v) != (e)) {f("'" var "' is 0x%x should be 0x%x\n", v, e); }

namespace mb
{
    char* na = "n/a";
    void init(multiboot_header_tag_address* mbi, uint32_t magic)
    {
        //logINF("Bootloader name: %s\n", (mbi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) ? (char*)mbi->boot_loader_name : na);
        //logINF("Mem start: 0x%x\nMem end: 0x%x\n", (mbi->flags & MULTIBOOT_INFO_MEMORY) ? mbi->mem_lower, mbi->mem_upper : 0, 0);   //lol this is vaild code
        printf("0x%x\n", (mbi->header_addr));
    }
}
