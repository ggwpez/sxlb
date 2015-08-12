#pragma once

#include "../types.hpp"

struct heap_header
{
	dword_t magic;		//4
	bool is_hole;		//4
    uint32_t footer_address;	//4
};	//12

struct heap_footer
{
	heap_header* header;	//4
	dword_t magic;			//4
};	//8

struct heap_header_info
{
	uint32_t size;
	heap_header* header;	//pointer heap_header in ram

	void set_zero();

	bool operator > (const heap_header_info& other) const;
	bool operator < (const heap_header_info& other) const;
	bool operator ==(const heap_header_info& other) const;
};
