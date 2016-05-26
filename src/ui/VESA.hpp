#ifndef VESA_HPP
#define VESA_HPP

#include "../types.hpp"

namespace2(ui, video)
{
    //see: http://wiki.osdev.org/Getting_VBE_Mode_Info

    typedef struct
    {
        uint8_t  vesa_signature[4]  __attribute__ ((packed));
        uint8_t vesa_version_l      __attribute__ ((packed));
        uint8_t vesa_version_h      __attribute__ ((packed));
        LPTR  oem_string_ptr        __attribute__ ((packed));
        uint32_t  capabilities      __attribute__ ((packed));
        LPTR  video_mode_ptr        __attribute__ ((packed));
        uint16_t memory             __attribute__ ((packed));
        uint16_t oem_software_rev   __attribute__ ((packed));
        LPTR  oem_vendor_name_ptr   __attribute__ ((packed));
        LPTR  oem_product_name_ptr  __attribute__ ((packed));
        LPTR  oem_product_rev_ptr   __attribute__ ((packed));
        uint8_t  reserved[222]      __attribute__ ((packed));
        uint8_t  oem_data[256]      __attribute__ ((packed));
    } VESA_info_t;

    typedef struct
    {
        uint16_t attributes;
        uint8_t win_a, win_b;
        uint16_t granularity;
        uint16_t win_size;
        uint16_t segment_a, segment_b;
        LPTR real_fptr;
        uint16_t pitch;

        uint16_t width, heigth;
        uint8_t w_char, h_char, planes, bpp, banks;
        uint8_t memory_model, bank_size, image_pages;
        uint8_t reserved0;

        uint8_t red_mask, red_position;
        uint8_t green_mask, green_position;
        uint8_t blue_mask, blue_position;
        uint8_t rsv_mask, rsv_position;
        uint8_t directcolor_attributes;

        uint32_t physbase;
        uint32_t offscreenmemoffset;
        uint16_t offscreenmemsize;
        uint8_t reserved[206];
    } VESA_mode_info_t __attribute__ ((packed));

    static_assert(sizeof(VESA_info_t) == 512, "VESA_info_t size wrong");
    static_assert(sizeof(VESA_mode_info_t) == 256, "VESA_mode_info_t size wrong");
}}
#endif // VESA_HPP
