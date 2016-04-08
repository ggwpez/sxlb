#include "multiboot.hpp"
#include "../ui/textmode.hpp"

#define assert(is, must, name) if (is != must) { logERR("Actual value for %s: 0x%x differs from 0x%x\n", name, is, must); } else { logtINF("%s = 0x%x", name, is);logDONE; }

namespace mb
{
    char* na = "n/a";
    void init(uint32_t ptr, uint32_t magic)
    {
        assert(magic, MULTIBOOT2_BOOTLOADER_MAGIC,  "GRUB2 boot magic");
        assert(ptr &7, 0, "mbi alignment &7");

        printfl("mbi size: 0x%x", *(uint32_t*)ptr);

        for (multiboot_tag* tag = (multiboot_tag*)(ptr +8);
             tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = (multiboot_tag*) ((multiboot_uint8_t*) tag               //warum bin ich hier
             + ((tag->size + 7) & ~7)))
        {
            switch (tag->type)
            {
                case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                    printfl("Boot loader name: %s", ((multiboot_tag_string*)tag)->string);
                    break;
            }
        }
    }
}
