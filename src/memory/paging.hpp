#pragma once

#include "../types.hpp"


struct page
{
	uint32_t swapped_in : 1;
	uint32_t access_right : 1;	//0: read-only,   1: read+write
	uint32_t access_ring : 1;	//0: kernel-ring, 1: user-ring
	uint32_t accessed : 1;
	uint32_t dirty : 1;			//sticky written bit
	uint32_t unused : 7;		//unused+reserved flags
	uint32_t frame_address : 20;//physical adress
}__attribute__((packed));

struct page_table
{
	struct page pages[1024];
}__attribute__((packed));

struct page_directory
{
	uint32_t tables_physical[1024];
	struct page_table* tables[1024];
	uint32_t physical_address;
}__attribute__((packed));

void					paging_install();
/**
 * @brief map_heap Mapps memory for the heap.
 * @return 0 on success. Otherwise returns the highest successfully mapped address.
 */
uint32_t                map_heap(uint32_t start, uint32_t end);
void					unmap_heap(uint32_t start, uint32_t end);
struct page*			get_page(uint32_t address, uchar_t make, struct page_directory* dir);
struct page*            set_page(uint32_t address, struct page_directory* dir);
uint32_t				alloc_frame(struct page* page, int is_kernel, int is_writeable);
void					free_frame(struct page* page);
uint32_t                k_malloc_no_heap(uint32_t size, uchar_t align, uint32_t* phys);
void                    enable_paging(page_directory* dir);

extern page_directory*  kernel_directory;
struct page_directory*	clone_directory(struct page_directory* src);
struct page_table*      clone_table(struct page_table* src);

uint32_t				show_physical_address(uint32_t virtual_address);
