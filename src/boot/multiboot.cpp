#include "multiboot.hpp"
#include "../ui/textmode.hpp"

#define assert(is, must, name) if (is != must) { logERR("Actual value for %s: 0x%x differs from 0x%x\n", name, is, must); } else { logtINF("%s = 0x%x", name, is);logDONE; }

namespace mb
{
    char* na = "n/a";
    void init(uint32_t ptr, uint32_t magic, ui::video::video_init_t* vdata)
    {
        multiboot_tag* tag;
        uint32_t size;

        assert(magic, MULTIBOOT2_BOOTLOADER_MAGIC,  "GRUB2 boot magic");
        assert(ptr &7, 0, "mbi alignment &7");

        printfl("mbi size: 0x%x", size = *(uint32_t*)ptr);

        for (tag = (multiboot_tag*)(ptr +8);
             tag->type != MULTIBOOT_TAG_TYPE_END;
             tag = (multiboot_tag*) ((multiboot_uint8_t*) tag               //warum bin ich hier
             + ((tag->size + 7) & ~7)))
        {
            switch (tag->type)
            {
                case MULTIBOOT_TAG_TYPE_CMDLINE:
                    printf ("Command line = %s\n",
                    ((struct multiboot_tag_string *) tag)->string);
                    break;
                case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                    printf ("Boot loader name = %s\n",
                    ((struct multiboot_tag_string *) tag)->string);
                    break;
                case MULTIBOOT_TAG_TYPE_MODULE:
                    printf ("Module at 0x%x-0x%x. Command line %s\n",
                    ((struct multiboot_tag_module *) tag)->mod_start,
                    ((struct multiboot_tag_module *) tag)->mod_end,
                    ((struct multiboot_tag_module *) tag)->cmdline);
                    break;
                case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                    printf ("mem_lower = %uKB, mem_upper = %uKB\n",
                    ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
                    ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                    break;
                case MULTIBOOT_TAG_TYPE_BOOTDEV:
                    printf ("Boot device 0x%x,%u,%u\n",
                    ((struct multiboot_tag_bootdev *) tag)->biosdev,
                    ((struct multiboot_tag_bootdev *) tag)->slice,
                    ((struct multiboot_tag_bootdev *) tag)->part);
                    break;
                case MULTIBOOT_TAG_TYPE_MMAP:
                {
                    multiboot_memory_map_t *mmap;
                    printf ("mmap\n");
                    for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                    (multiboot_uint8_t *) mmap
                    < (multiboot_uint8_t *) tag + tag->size;
                    mmap = (multiboot_memory_map_t *)
                    ((unsigned long) mmap
                    + ((struct multiboot_tag_mmap *) tag)->entry_size))
                    printf (" base_addr = 0x%x%x,"
                    " length = 0x%x%x, type = 0x%x\n",
                    (unsigned) (mmap->addr >> 32),
                    (unsigned) (mmap->addr & 0xffffffff),
                    (unsigned) (mmap->len >> 32),
                    (unsigned) (mmap->len & 0xffffffff),
                    (unsigned) mmap->type);
                }
                break;
                case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                {
                    multiboot_uint32_t color;
                    unsigned i;
                    struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *) tag;
                    void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

                    switch (tagfb->common.framebuffer_type)
                    {
                        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                        {
                            unsigned best_distance, distance;
                            struct multiboot_color *palette;
                            palette = tagfb->framebuffer_palette;
                            color = 0;
                            best_distance = 4*256*256;
                            for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                            {
                                distance = (0xff - palette[i].blue)
                                * (0xff - palette[i].blue)
                                + palette[i].red * palette[i].red
                                + palette[i].green * palette[i].green;
                                if (distance < best_distance)
                                {
                                    color = i;
                                    best_distance = distance;
                                }
                            }
                        } break;
                        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                            color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) << tagfb->framebuffer_blue_field_position;
                            break;
                        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                            color = '\\' | 0x0100;
                            break;
                        default:
                            color = 0xfffffff;
                            break;
                    }

                    vdata->bpp = tagfb->common.framebuffer_bpp;
                    vdata->bypp = vdata->bpp >> 3;
                    vdata->w = tagfb->common.framebuffer_width;
                    vdata->h = tagfb->common.framebuffer_height;
                    vdata->pitch = tagfb->common.framebuffer_pitch;
                    vdata->fb = fb;

                    logINF("Resolution: %ux%ux%u\n", vdata->w, vdata->h, vdata->bpp);

                    break;
                }
            }
        }

        logtINF("Total mbi size 0x%x", (unsigned) tag - ptr);
        logOK;
    }
}
