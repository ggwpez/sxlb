#include "paging.hpp"
#include "memory.hpp"
#include "../ui/textmode.hpp"

#define HEAP_INDEX_SIZE     0x20000
#define HEAP_MIN_SIZE       0x70000
#define PHYSICAL_MEMORY  0x20000000

using namespace memory;

heap kheap = *(heap*)0;
uint32_t   NFRAMES = (PHYSICAL_MEMORY / PAGE_SIZE);
uint32_t*  frames; // pointer to the bitset (functions: set/clear/test)
uint32_t   ind, offs;

page_directory* kernel_directory, *current_directory;
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
/***********************************************************/

internal uint32_t first_frame() // find the first free frame in frames bitset
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

uint32_t alloc_frame(page* page_, int is_kernel, int is_writeable) // allocate a frame
{
	if (!(page_->frame_address))
	{
		uint32_t index = first_frame(); // search first free page frame

		set_frame(index*PAGE_SIZE);

		page_->swapped_in = 1;
		page_->access_right = (is_writeable == 1) ? 1 : 0;
		page_->access_ring = (is_kernel == 1) ? 0 : 1;
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
    frames = (uint32_t*)k_malloc_no_heap(NFRAMES / 32, 0, 0);
    memory::memset(frames, 0, NFRAMES / 32);
};

page* set_page(uint32_t address, page_directory* dir)
{
	//ULONG tmp = address;
	address >>= 12; // /= PAGESIZE        // index <== address
	uint32_t table_index = (address >> 10);  // ==> page table containing this address

	//TODO: check it against Intel Manual 3A !!!
    if ((((uint32_t)(dir->tables_physical[table_index])) & 0x1)
        && (dir->tables[table_index]))
    {}
	else
	{
		uint32_t phys;
        dir->tables[table_index] = (page_table*)k_malloc_no_heap(sizeof(page_table), 1, &phys);
        memory::memset(dir->tables[table_index], 0, PAGE_SIZE);
		dir->tables_physical[table_index] = phys | 0x7; // 111b meaning: PRESENT=1, RW=1, USER=1
	}
	return &dir->tables[table_index]->pages[address % 1024];
};

uint32_t map_heap(uint32_t start, uint32_t end)
{
	uint32_t i = 0;
	for (i = start; i < end; i += PAGE_SIZE)
        alloc_frame(set_page(i, kernel_directory), 0, 0);
        //if (alloc_frame(set_page(i, kernel_directory), 0, 0) == 0)
          //  return i;

    return 0;
};

void unmap_heap(uint32_t start, uint32_t end)
{
	uint32_t i = 0;
	for (i = start; i < end; i -= PAGE_SIZE)
		free_frame(get_page(i,0, kernel_directory));
};

void heap_install()
{
    kheap = heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX, KHEAP_START + HEAP_MIN_SIZE, 1, 1);
    kheap_set = true;
};

void paging_install()
{
	bitset_install();

	// make a page directory
	uint32_t phys;
    kernel_directory = (page_directory*)k_malloc_no_heap(sizeof(page_directory), 1, &phys);
    memory::memset(kernel_directory, 0, sizeof(page_directory));
	kernel_directory->physical_address = phys;    

	uint32_t counter = 0, i = 0, index = 0;
	while (i < (placement_address + 0x10000))
	{
        page* page = set_page(i, kernel_directory);
		if (((i >= 0xb8000) && (i <= 0xbf000)) || ((i >= 0xd000) && (i < 0xe000)))
		{
			index = alloc_frame(page, 0, 1); // exclude VRAM
		}
		else
			index = alloc_frame(page, 1, 0);
		i += PAGE_SIZE; ++counter;
	}

    map_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE);
    current_directory = clone_directory(kernel_directory, nullptr);

    enable_paging(kernel_directory);

    heap_install();
};

void enable_paging(page_directory* dir)
{
    current_directory = dir;

    // cr3: PDBR (Page Directory Base Register)
    asm volatile("mov %0, %%cr3":: "r"(current_directory->physical_address));    //set page directory base pointer
    // read cr0, set paging bit, write cr0 back
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));   // read cr0
    cr0 |= 0x80000000;                          // set the paging bit in CR0 to enable paging
    asm volatile("mov %0, %%cr0":: "r"(cr0));   // write cr0
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
	address /= PAGE_SIZE;                // address ==> index.
	uint32_t table_index = address / 1024; // ==> page table containing this address

	if (dir->tables[table_index])       // table already assigned
	{
		return &dir->tables[table_index]->pages[address % 1024];
	}
	else if (make)
	{
		uint32_t phys;
        dir->tables[table_index] = (page_table*)k_malloc_no_heap(sizeof(page_table), 1, &phys);
        memory::memset(dir->tables[table_index], 0, PAGE_SIZE);
		dir->tables_physical[table_index] = phys | 0x7; // 111b meaning: PRESENT=1, RW=1, USER=1
		return &dir->tables[table_index]->pages[address % 1024];
	}
	else
		return 0;
};

uint32_t show_physical_address(uint32_t virtual_address)
{
    page* page = get_page(virtual_address, 0, kernel_directory);
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
		}
	}
};

page_table* clone_table(page_table* source, uint32_t* physAddr)
{
    //### only testing without heap, because heap dosent support alinged allocations yet
    uint32_t phys;

    LPTR tmp = memory::k_malloc(sizeof(page_directory) + PAGE_SIZE, 0, &phys);
    uint32_t offset = PAGE_SIZE - tmp%PAGE_SIZE;
    LPTR addr = tmp + offset;

    printfl("mod table: %i", addr % PAGE_SIZE);

    page_table* ret = (page_table*)addr;

    memory::memset(ret, 0, sizeof(page_table));

    for (uint32_t i = 0; i < 1024; ++i)
    {
        if (!source->pages[i].frame_address)
            continue;

        alloc_frame(&ret->pages[i], 0, 0);

        if (source->pages[i].swapped_in)    ret->pages[i].swapped_in = 1;
        if (source->pages[i].access_right)  ret->pages[i].access_right = 1;
        if (source->pages[i].access_ring)   ret->pages[i].access_ring = 1;
        if (source->pages[i].accessed)      ret->pages[i].accessed = 1;
        if (source->pages[i].dirty)         ret->pages[i].dirty = 1;

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

    memory::memset(dir, 0, sizeof(page_directory));
    dir->physical_address = phys + off;

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
            dir->tables_physical[i] = phys | 0x07;
        }
    }

    return dir;
};
