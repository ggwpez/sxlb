#include "memory.hpp"
#include "system.hpp"
#include "ctor.cpp"

extern uint32_t placement_address;
extern page_directory* kernel_directory;
extern heap kheap;
extern bool kheap_set;

namespace memory
{
    void init()
    {
        paging_install();

        if (!kheap_set)
            syshlt("HEAP allocation failed.");
    }

    void dump_info(struct ::heap* heape)
    {
        if (!kheap_set)
            kheap.dump_info();
        else
            heape->dump_info();
   };

    void* memset(void* dest, char val, size_t count)
    {
        char* temp = (char*)dest;
        for (; count != 0; count--) *(temp++) = val;
        return dest;
    };

    void* memcpy(void* dest, void* source, size_t count)
    {
        while (count--)
            *(byte_t*)dest++ = *(byte_t*)source++;

        return dest;
    };

    bool f_locked = false;
    uint32_t k_free(void* ptr)
    {
        //while (f_locked); f_locked = true;

        if (!ptr)
        {
            f_locked = false;
            syshlt("null free");
        }

        if (kheap_set)
        {
            uint32_t tmp = kheap.free(ptr);
            f_locked = false;

            if (tmp)
            {
                printlf("Freed: %M", tmp);
                return tmp;
            }
            else
            {
                printlf("freeing failed 0");
                return 0;
            }
        }
        else
        {
            f_locked = false;
            printlf("freeing failed 1");
            return 0;
        }
    };

    bool m_locked = false;
    uint32_t k_malloc(uint32_t size, uchar_t align, uint32_t* phys)
    {
        if (kheap_set)
        {
            //while (m_locked); m_locked = true;

            void* ret = kheap.malloc(size, align);
            if (phys)
            {
                struct page* page = get_page((uint32_t)ret, 0, kernel_directory);
                *phys = page->frame_address*PAGE_SIZE + ((uint32_t)ret & 0x00000FFF);
            }
            if (ret)
                printlf("Allocated %M", size);

            m_locked = false;
            return ret;
        }

        if (align == 1)
        {
            if (!(placement_address == (placement_address & 0xFFFFF000)))
            {
                placement_address &= 0xFFFFF000;
                placement_address += PAGE_SIZE;
            }
        }

        if (phys)
        {
            *phys = placement_address;
        }
        uint32_t temp = placement_address;
        placement_address += size;     // new placement_address is increased

        m_locked = false;
        return temp;                   // old placement_address is returned
    };
}
