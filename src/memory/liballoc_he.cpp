#include "liballoc.hpp"
#include "paging.hpp"
#include "memory.hpp"
#include "../system/system.hpp"

static volatile bool locked = false;
int liballoc_lock()
{
    cli
    while (locked); locked = true;
}

int liballoc_unlock()
{
    locked = false;
    sti
}

uint32_t page_ptr = KHEAP_START;
void* liballoc_alloc(int s)
{
    uint32_t end = page_ptr +(s *PAGE_SIZE);
    map_heap(page_ptr, end);
    uint32_t ret = page_ptr;
    page_ptr = end;
    return ret;
}

int liballoc_free(void* start, int c)
{
    /*if (start +(c *PAGE_SIZE) != page_ptr)
    {
        logERR("Internal error in liballoc_free start: 0x%x\nc: 0x%x\npage_ptr: 0x%x\nadd: 0x%x", start, c, page_ptr, start +(c *PAGE_SIZE));
        stop
    }*/
    page_ptr -= c *PAGE_SIZE;

    return unmap_heap(start, (uint32_t)start +(c *PAGE_SIZE));
    return 0;
}
