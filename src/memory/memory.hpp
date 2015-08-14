#pragma once

#include "paging.hpp"
#include "heap.hpp"
#include "../types.hpp"

namespace memory
{
    #define KHEAP_START         0x01000000
    #define KHEAP_INITIAL_SIZE  _1MiB + sizeof(heap_header) + sizeof(heap_footer)	//otherwise its a bit smaller than expected
    #define KHEAP_MAX           KHEAP_START + _4MiB
    #define HEAP_MIN_SIZE		KHEAP_INITIAL_SIZE

    class heap;
    void init();
    void dump_info(heap* heape);
    uint32_t k_malloc(uint32_t size);
    uint32_t k_malloc(uint32_t size, uchar_t align, uint32_t* phys);	//you better care about the return value, and free the memory!
    uint32_t k_free(void* ptr);
    void* memset(void* dest, char val, size_t count);
    void* memcpy(void* dest, void* source, size_t count);
}