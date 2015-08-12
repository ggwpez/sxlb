#pragma once

#include "memory/heap_he.hpp"
#include "types.hpp"

class ordered_array
{
public:
    int32_t size, capacity;
    heap_header_info* data;
	ordered_array();
    ordered_array(uint32_t Capacity);
	~ordered_array();
	/*Use if it is nearly sorted.*/
	void best_case_order();
	/*Use if the array is not sorted at all.*/
	void worst_case_order();

    heap_header_info* find_fitting_block(uint32_t size, bool page_aligned, uint32_t* index_out);
	heap_header_info* find_by_address(uint32_t address);

	bool add(heap_header_info value);
    bool exists(heap_header* address);
	bool remove_by_index(uint32_t index);
    bool remove_by_address(heap_header* address);
    bool remove_by_address(heap_header_info* address);

	/*If this returns false, you better halt the system and go debugging!*/
	bool check_size();
private:
	uint32_t	element_size;
};
