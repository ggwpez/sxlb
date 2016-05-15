#include "paging.hpp"
#include "memory.hpp"
#include "../ui/textmode.hpp"

#define FLAGS B(111)
#define PHYSICAL_MEMORY  0x80000000

using namespace memory;

heap    kheap = *(heap*)0;
uint32_t   NFRAMES = (PHYSICAL_MEMORY / PAGE_SIZE);
uint32_t*  frames; // pointer to the bitset (functions: set/clear/test)
uint32_t   ind, offs;

page_directory* kernel_directory,* current_directory;
bool kheap_set = false;

//page_directory* current_directory = 0;

extern "C"
{
    extern copy_page_physical(LPTR arg0, LPTR arg1);
}

uint32_t placement_address = _2MiB;

uint32_t k_malloc_no_heap(uint32_t size, uchar_t align, uint32_t* phys)
{
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
    return temp;                   // old placement_address is returned
};

/************* bitset variables and functions **************/

internal void get_Index_and_Offset(uint32_t frame_addr)
{
    uint32_t frame = frame_addr / PAGE_SIZE;
    ind = frame / 32;
    offs = frame % 32;
};

internal void set_frame(uint32_t frame_addr)
{
    get_Index_and_Offset(frame_addr);
    frames[ind] |= (1 << offs);
};

internal void clear_frame(uint32_t frame_addr)
{
    get_Index_and_Offset(frame_addr);
    frames[ind] &= ~(1 << offs);
};

/*
static uint32_t test_frame(uint32_t frame_addr)
{
    get_Index_and_Offset(frame_addr);
    return( frames[ind] & (1<<offs) );
}
*/

uint32_t first_frame() // find the first free frame in frames bitset
{
    uint32_t index, offset;
    for (index = 0; index<(NFRAMES / 32); ++index)
    {
        if (frames[index] != (uint32_t)-1)
        {
            for (offset = 0; offset<32; ++offset)
            {
                if (!(frames[index] & 1 << offset)) // bit set to zero?
                    return (index * 32 + offset);
            }
        }
    }
    return (uint32_t)-1; // no free page frames
};

uint32_t alloc_frame(page* page_, int user, int read_write) // allocate a frame
{
    if (!(page_->frame_address))
    {
        uint32_t index = first_frame(); // search first free page frame

        set_frame(index*PAGE_SIZE);

        page_->swapped_in = 1;
        page_->user = user;
        page_->read_write = read_write;
        page_->frame_address = index;
        return index;
    }
    return 0;
};

void free_frame(page* page) // dellocate a frame
{
    if (page->frame_address)
    {
        clear_frame(page->frame_address);
        page->frame_address = 0;
    }
};

internal void bitset_install()
{
    logtINF("installing frames...");
    frames = (uint32_t*)k_malloc_no_heap(NFRAMES / 32, 0, 0);
    memory::memset(frames, 0, NFRAMES / 32);
    logINF("(%u)", NFRAMES / 32);logDONE;
};

uint32_t map_heap(uint32_t start, uint32_t end)
{
    uint32_t i = 0;
    for (i = start; i < end; i += PAGE_SIZE)
        alloc_frame(get_page(i, 1, kernel_directory), US, RW);
        //if (alloc_frame(set_page(i, kernel_directory), SV, RW) == 0)
          //  return i;

    return 0;
};

int unmap_heap(uint32_t start, uint32_t end)
{
    uint32_t i = 0;
    for (i = start; i < end; i += PAGE_SIZE)
        free_frame(get_page(i, 0, kernel_directory));

    return end == start ? 0: (end -start) /PAGE_SIZE;
};

void heap_install()
{
    kheap = heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX, KHEAP_START + HEAP_MIN_SIZE, 1, 1);
    kheap_set = true;
};

void paging_install()
{
    bitset_install();

    logtINF("creating page dirs + mapping memory...");
    kernel_directory = (page_directory*)k_malloc_no_heap(sizeof(page_directory), 1, nullptr);
    memory::memset(kernel_directory, 0, sizeof(page_directory));
    kernel_directory->physical_address = kernel_directory->tables_physical;

    uint32_t counter = 0, i = 0, index = 0;

    for( i=KHEAP_START; i<KHEAP_START+KHEAP_INITIAL_SIZE; i+=0x1000 )
        get_page(i, 1, kernel_directory);

    i = 0;
    while (i < (placement_address + 0x10000))
    {
        if ((i >= 0xa0000 && i <= 0xbffff) || ((i >= 0xb8000) && (i <= 0xbf000)) || ((i >= 0xd000) && (i < 0xe000)))
        {
            index = alloc_frame(get_page(i, 1,kernel_directory), US, RW); // exclude VRAM
        }
        else
            index = alloc_frame(get_page(i, 1,kernel_directory), US, RW);
        i += PAGE_SIZE; ++counter;
    }

    uint32_t user_space_start = 0x400000;
    uint32_t user_space_end   = 0x500000;
    i=user_space_start;
    while( i < user_space_end )
    {
        alloc_frame( get_page(i, 1, kernel_directory), US, RW); // user and read-only
        i += PAGE_SIZE;
    }

    map_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE);
    current_directory = clone_directory(kernel_directory, nullptr);
    logDONE;

    enable_paging(kernel_directory);

    heap_install();
};

void enable_paging(page_directory* dir)
{
    logtINF("enabling pagin...");
    // cr3: PDBR (Page Directory Base Register)
    asm volatile("mov %0, %%cr3":: "r"(dir->physical_address));    //set page directory base pointer
    // read cr0, set paging bit, write cr0 back
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));   // read cr0
    cr0 |= 0x80000000;                          // set the paging bit in CR0 to enable paging
    asm volatile("mov %0, %%cr0":: "r"(cr0));   // write cr0
    logDONE;
}

void switch_paging(page_directory* dir)
{
    if (current_directory == dir)
        return;

    current_directory = dir;
    asm volatile("mov %0, %%cr3" : : "r"(current_directory->physical_address));
};

page* get_page(uint32_t address, uchar_t make, page_directory* dir)
{
    address /= PAGE_SIZE;                   // address ==> index.
    uint32_t table_index = address / 1024;  // ==> page table containing this address

    if (dir->tables[table_index])           // table already assigned
    {
        return &dir->tables[table_index]->pages[address % 1024];
    }
    else if (make)
    {
        uint32_t phys;
        dir->tables[table_index] = (page_table*)k_malloc_no_heap(sizeof(page_table), 1, &phys);
        memory::memset(dir->tables[table_index], 0, PAGE_SIZE);
        dir->tables_physical[table_index] = phys | FLAGS;       // 111b meaning: PRESENT=1, RW=1, USER=1
        return &dir->tables[table_index]->pages[address % 1024];
    }
    else
        return 0;
};

uint32_t show_physical_address(uint32_t virtual_address)
{
    page* page = get_page(virtual_address, 0, current_directory);
    return((page->frame_address)*PAGE_SIZE + (virtual_address & 0xFFF));
};

void analyze_physical_addresses()
{
    int i, j, k = 0, k_old;
    for (i = 0; i<(PHYSICAL_MEMORY / 0x18000 + 1); ++i)
    {
        for (j = i * 0x18000; j<i * 0x18000 + 0x18000; j += 0x1000)
        {
            if (show_physical_address(j) == 0)
            {
                k_old = k; k = 1;
            }
            else
            {
                if (show_physical_address(j) - j)
                {
                    k_old = k; k = 2;
                }
                else
                {
                    k_old = k; k = 3;
                }
            }
            if(k!=k_old)
                printf("%x %x\n", j, show_physical_address(j));
        }
    }
};

page_table* clone_table(page_table* source, uint32_t* physAddr)
{
    //### only testing without heap, because heap dosent support alinged allocations yet
    LPTR addr = memory::k_malloc(sizeof(page_table), 0, physAddr);

    page_table* ret = (page_table*)addr;

    memory::memset(ret, 0, sizeof(page_table));

    for (uint32_t i = 0; i < 1024; ++i)
    {
        if (!source->pages[i].frame_address)
            continue;

        alloc_frame(&ret->pages[i], 0, 0);

        ret->pages[i].swapped_in = source->pages[i].swapped_in;
        ret->pages[i].read_write = source->pages[i].read_write;
        ret->pages[i].user = source->pages[i].user;
        ret->pages[i].accessed = source->pages[i].accessed;
        ret->pages[i].dirty = source->pages[i].dirty;

        copy_page_physical(source->pages[i].frame_address*0x1000, ret->pages[i].frame_address*0x1000);
    };

    return ret;
};

page_directory* clone_directory(page_directory* src, uint32_t* dir_offset)
{
    uint32_t phys, off;
    LPTR tmp = memory::k_malloc(sizeof(page_directory) + PAGE_SIZE, 0, &phys);
    off = PAGE_SIZE - tmp % PAGE_SIZE;
    page_directory* dir = (page_directory*)(tmp + off);

    if (dir_offset)
        *dir_offset = off;

    memory::memcpy(dir, src, sizeof(page_directory));
    dir->physical_address = src->physical_address;

    /*if (dir_offset)
        *dir_offset = off;

    uint32_t offset = (uint32_t)dir->tables_physical - (uint32_t)dir;
    memory::memset(dir, 0, sizeof(page_directory));
    dir->physical_address = phys + off + offset;

    for (uint32_t i = 0; i < 1024; ++i)
    {
        if (!src->tables[i])    //### also try the other table
            continue;

        if (kernel_directory->tables[i] == src->tables[i])  //is it already mapped in the kernel_directory?
        {
            dir->tables[i] = src->tables[i];
            dir->tables_physical[i] = src->tables_physical[i];
        }
        else    //no, have to copy it
        {
            uint32_t phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tables_physical[i] = phys | FLAGS;
        }
    }*/

    return dir;
};
