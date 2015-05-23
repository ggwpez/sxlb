#include "memory.hpp"
#include "system.hpp"
#include "ctor.cpp"

heap  kheap;				//kernel heap
heap* kheap_p = nullptr;

extern uint32_t placement_address;
extern struct page_directory* kernel_directory;

void sxlb_memory_init()
{
	paging_install();

	map_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE);

	kheap = heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX, KHEAP_START + HEAP_MIN_SIZE, 1, 1);
	kheap_p = &kheap;
}

void sxlb_memory_dump_info(heap* heape)
{
	if (!heape)
		heape = kheap_p;

	kheap_p->dump_info();
};

/*void* memset(void* dest, char val, size_t count)
{
	char* temp = (char*)dest;
	for (; count != 0; count--) *(temp++) = val;
	return dest;
};*/

void* memcpy(__restrict__ byte_t* dest, __restrict__ const byte_t* source, size_t count)
{
	while (count--)
		*dest++ = *source++;

	return dest;
};

uint32_t k_free(void* ptr)
{
	if (!ptr)
		return false;

	if (kheap_p)
	{
		uint32_t tmp = kheap.free(ptr);
		if (tmp)
		{
			printlf("Freed: %M", tmp);
			return tmp;
		}	
		else 
			return 0;
	}
	else
		return 0;
};

uint32_t k_malloc(uint32_t size)
{
	return k_malloc(size, 0, nullptr);
};

uint32_t k_malloc(uint32_t size, uchar_t align, uint32_t* phys)
{
	if (kheap_p)
	{
		//kernel-heap enabled
		//printlf("\r\nonce");
		void* ret = kheap.malloc(size);
		if (phys)
		{
			struct page* page = get_page((uint32_t)ret, 0, kernel_directory);
			*phys = page->frame_address*PAGE_SIZE + ((uint32_t)ret & 0x00000FFF);
		}
		if (ret)
			printlf("Allocated %M", size);
			
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
	return temp;                   // old placement_address is returned
};
