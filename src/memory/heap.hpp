#pragma once

#include "../array_or.hpp"
//#include "types.hpp"

namespace memory
{

class heap
{
public:
    heap();
	heap(uint32_t Start_address, uint32_t End_address, uint32_t Max_address, uint32_t Min_address, uint8_t Access_ring, uint8_t Access_rigth);

    void* malloc(uint32_t size);
    void* malloc(uint32_t size, bool page_aligned);
    void* malloc_pa(uint32_t size);
	uint32_t free(void* ptr);
	void dump_info();
	heap_header* search_before(heap_header* address);
	heap_header* search_after (heap_footer* address);

	void contract(uint32_t until);
    bool expand(uint32_t to);

	void install_footer(uint32_t address, heap_header* header);
	void install_header(uint32_t address, bool is_hole, heap_footer* footer);

    ~heap();

	ordered_array list;
	//uint32_t size, capacity;
	uint32_t start_address;
	uint32_t end_address;
	uint32_t min_address;
	uint32_t max_address;

	uint32_t access_right : 1;	//0: read-only,   1: read+write
	uint32_t access_ring : 1;	//0: kernel-ring, 1: user-ring
};
}
