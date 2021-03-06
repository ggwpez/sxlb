#include "memory.hpp"
#include "../system/system.hpp"
#include "../ctor.hpp"
#include "liballoc.hpp"

using namespace memory;
extern uint32_t placement_address;
extern page_directory* kernel_directory;
extern heap kheap;
extern bool kheap_set;

namespace memory
{
    void init(uint32_t vbuff, uint32_t vbuff_len)
    {
        paging_install(vbuff, vbuff_len);

        if (!kheap_set)
            syshlt("HEAP allocation failed.");
    }

    void dump_info(heap* heape)
    {
        if (kheap_set)
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

    void* memset32(void* dest, int32_t val, size_t count)
    {
        int32_t* temp = (int32_t*)dest;
        for (; count != 0; count--) *(temp++) = val;
        return dest;
    }

    void* memcpy(void* dest, void* source, size_t count)
    {
        if (!(((uint32_t)dest & B(111)) | ((uint32_t)source & B(111)) | (count & B(111))))      //8 byte aligned, thats also faster on IA32, due to better optimized instructions and less while loop
        {
            count >>= 3;

            while (count--)
               *(((qword_t*&)dest))++ = *(((qword_t*&)source)++);
        }
        else if (!(((uint32_t)dest & B(11)) | ((uint32_t)source & B(11)) | (count & B(11))))    //4 byte alinged
        {
            count >>= 2;

            while (count--)
               *(((dword_t*&)dest))++ = *(((dword_t*&)source)++);
        }
        else if (!(((uint32_t)dest & 1) | ((uint32_t)source & 1) | (count & 1)))                //2 byte alinged
        {
            count >>= 1;

            while (count--)
               *(((word_t*&)dest))++ = *(((word_t*&)source)++);
        }
        else                                                                                    //not alinged, use byte copy
        {
            while (count--)
               *(byte_t*)dest++ = *(byte_t*)source++;
        }

        return dest;
    };

    bool f_locked = false;
    uint32_t k_free(void* ptr)
    {
        //while (f_locked); f_locked = true;

        lib_free(ptr);
        return 0;
        /*if (!ptr)
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
                //printfl("Freed(1): %M", tmp);
                return tmp;
            }
            else
            {
                syshlt("freeing failed 0");
                return 0;
            }
        }
        else
        {
            f_locked = false;
            syshlt("kheap_set == 0");
            return 0;
        }//*/
    };

    bool m_locked = false;
    uint32_t k_malloc(uint32_t size, uchar_t align, uint32_t* phys)
    {
        if (!size)
            return 0;

        if (align)
            syshlt("No aligned allocations.");

        if (kheap_set)
        {
            //while (m_locked); m_locked = true;

            //void* ret = kheap.malloc(size, align);
            void* ret = lib_malloc(size);

            if (phys)
            {
                page* page = get_page((uint32_t)ret, 0, kernel_directory);
                *phys = page->frame_address*PAGE_SIZE + ((uint32_t)ret & 0x00000FFF);
            }

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
        return temp;                   // old placement_address is returned*/
    };
}
