#include "heap.hpp"
#include "../system/system.hpp"
#include "memory.hpp"

namespace memory
{
#define MAGIC 'lmao'

#define OVERHEAD sizeof(heap_header)+sizeof(heap_footer)

heap::heap()
{ }

heap::heap(uint32_t Start_address, uint32_t End_address, uint32_t Max_address, uint32_t Min_address, uint8_t Access_ring, uint8_t Access_rigth)
{
    logtINF("initalizing heap...");
#if __CHECKS_ADVNCD
    if (Start_address + OVERHEAD > End_address || Max_address < End_address)
        syshlt("HEAP initialization error!");
#endif
                                                //sizeof(heap_header_info)*2000 = 16 Kb
    this->list = ordered_array(2000);			//(sizeof(heap_header) + sizeof(heap_footer))*2000 = 40 Kb
    this->access_ring = Access_ring;
    this->access_right = Access_rigth;
    this->start_address = Start_address;
    this->end_address = End_address;
    this->min_address = Min_address;
    this->max_address = Max_address;

    //printfl("heapStart@: %u  heapSize: %u", this->start_address, this->end_address -this->start_address);
    uint32_t footer_address = this->end_address - sizeof(heap_footer);
    this->install_header(this->start_address, true, footer_address);
    this->install_footer(footer_address, this->start_address);

    heap_header_info tmp;
    tmp.header = (heap_header*)this->start_address;
    tmp.size = this->end_address - this->start_address - sizeof(heap_header)-sizeof(heap_footer);
    //printfl("block0@: %u  block0Size: %u", tmp.header, tmp.size);

    if (!tmp.header)
        syshlt("HEAP out of memory!");		//could not be allocated

    this->list.add(tmp);

    if (this->list.size != 1)
        syshlt("HEAP internal error! 20");
    logINF("(len: 0x%x addr: 0x%x)", this->end_address - this->start_address, this->start_address); logDONE;
}

void heap::dump_info()
{
    printfl("Free blocks: %i  Start: %M  End: %M  Max: %M", this->list.size, this->start_address, this->end_address, this->max_address);
    for (uint32_t i = 0; i < this->list.size; i++)
    {
        printfl("(%u)  S: %M  A: %M  E: %M  F: %b", i, this->list.data[i].size, this->list.data[i].header, this->list.data[i].header->footer_address, this->list.data[i].header->is_hole);
    }

    /*
     * if (this->list.size == 1)
        printfl("No allocations. Start: %M  End: %M  Max: %M", this->start_address, this->end_address, this->max_address);
    else
    {
        printfl("Allocations: %i  Start: %M  End: %M  Max: %M", this->list.size, this->start_address, this->end_address, this->max_address);

        if (this->list.size < 10)
        for (uint32_t i = 0; i < this->list.size; i++)
            printfl("n: %i  S: %M  A: %M  E: %M  H: %b", i, this->list.data[i].size, this->list.data[i].header, this->list.data[i].header->footer_address, this->list.data[i].header->is_hole);
    }
     */
}

/*Create a footer at the given address*/
void heap::install_footer(uint32_t address, heap_header* header)
{
    if (address + sizeof(heap_footer) > this->max_address)
        syshlt("HEAP internal error! 14");

    heap_footer* tmp = (heap_footer*)address;

    tmp->header = header;
    tmp->magic = MAGIC;
}

void heap::install_header(uint32_t address, bool is_hole, heap_footer* footer)
{
    if (address < this->start_address)
        syshlt("HEAP internal error! 15");

    heap_header* tmp = (heap_header*)address;

    tmp->magic = MAGIC;
    tmp->is_hole = is_hole ? 1 : 0;
    tmp->footer_address = (uint32_t)footer;
}

/*Allocated memory page aligned.*/
/*void* heap::malloc_pa(uint32_t size)
{
    LPTR ptr = malloc(size + PAGE_SIZE*2);

    if (!ptr)
        return nullptr;

    uint32_t offset = PAGE_SIZE - ptr%PAGE_SIZE;



    return ptr + offset;
}*/

void* heap::malloc(uint32_t size)
{ return malloc(size, false); }

void* heap::malloc(uint32_t size, bool page_aligned)
{
    if (page_aligned) syshlt("Aligned allocations not allowed.");

    uint32_t index = 0;
    heap_header_info* found = this->list.find_fitting_block(size, page_aligned, &index);

    heap_footer* eie = this->end_address - sizeof(heap_footer);
    if (eie->magic != MAGIC || !eie->header)
    {
        printfl("\neie %M   %M   %M  s: %M", (uint32_t)sizeof(heap_footer), eie->header, eie->magic, this->end_address -sizeof(heap_footer));
        this->dump_info();
        syshlt("HEAP Magic error! 0");
    }

#if __CHECKS_DBG
    if (!&index)
        syshlt("HEAP internal error! 9");
#endif

#if __CHECKS_ADVNCD
    if (this->end_address >= this->max_address)
        syshlt("HEAP internal overflow!");
#endif

    if (!found) //no block found? check if we can expand the last block, or have to expand and create a new one
    {
        if ((this->end_address +size +OVERHEAD) >= this->max_address)
            syshlt("HEAP out of memory! 0");

        heap_footer* found_footer = this->end_address - sizeof(heap_footer);
        heap_header* found_header = found_footer->header;

        if (found_footer->magic != MAGIC || found_header->magic != MAGIC)
            syshlt("HEAP magic error. 0");

        //heap_header_info* found_info = list.find_by_address(found_header);

        if (found_header->is_hole)	//nice, we can easily expand this block
        {
            uint32_t found_size = found_header->footer_address - (uint32_t)found_header;
            uint32_t missing_size = found_size - size;
            uint32_t new_footer;
            //found_footer->magic = 0;

            //printfl("found: %u, needed: %u, missing: %u", found_header->footer_address - (uint32_t)found_header, size, missing_size);
            expand(this->end_address + missing_size);

            new_footer = this->end_address - sizeof(heap_footer);

            install_footer(new_footer, found_header);
            found_header->footer_address = new_footer;
            found_header->is_hole = false;
            list.remove_by_address(found_header);

            return (void*)((uint32_t)found_header +sizeof(heap_header));
        }
        else						//have to create a new one
        {
            uint32_t old_end = this->end_address;

            expand(this->end_address +size +OVERHEAD);

#if __CHECKS_DBG
            if (this->end_address - old_end != size + OVERHEAD)
                syshlt("HEAP internal error! 12");
#endif
            install_header(old_end, false, this->end_address - sizeof(heap_footer));
            install_footer(this->end_address -sizeof(heap_footer), old_end);

            return (void*)(old_end +sizeof(heap_header));
        }
    }
    else if (found->size == size)	//found block matches exaclly the requested size - nice
    {
        heap_header* header = found->header;
        header->is_hole = false;
        //header->magic = 0;
        //### maybe check if the heap must be expanded? - nope, will be handled on the next allocation
        uint32_t ret = (uint32_t)found->header + sizeof(heap_header);	//save it

        list.remove_by_address(found);
        list.best_case_order();

        return (void*)ret;
    }
    else					//block too big, have to split it up in to two smaller ones
    {
        uint32_t d_size = found->size - size;
        //printfl("f_size-size: %u-%u = %u", found->size, size, d_size);

        heap_header* found_header = found->header;
        heap_footer* found_footer = (heap_footer*)found_header->footer_address;

        if (found_header->magic != MAGIC || found_footer->magic != MAGIC || !found_header->is_hole)
            syshlt("HEAP magic error. 3");

        found_header->footer_address = (uint32_t)found_header + size +sizeof(heap_header);
        found_header->is_hole = false;
        install_footer(found_header->footer_address, found_header);
        //printfl("footer@: %u", found_header->footer_address);

        if (d_size <= OVERHEAD)
        {
            list.remove_by_address(found);	//the new block is to small to hold user data, have to let it free
            list.best_case_order();
            //printfl("Im not willed to waste memory!");
            contract(found_header->footer_address +sizeof(heap_footer));	//free the memory, that this block otherwise would have used
#if __CHECKS_DBG
            if (size != (found_header->footer_address - (uint32_t)found_header - sizeof(heap_header)))
                syshlt("HEAP internal error! 11");
#endif
            return (uint32_t)found_header + sizeof(heap_header);
        }

        uint32_t new_header = found_header->footer_address + sizeof(heap_footer);
        found_footer->header = (heap_header*)new_header;
        install_header(new_header, true, found_footer);
        //install_footer(found_footer, new_header);

        uint32_t ret = (uint32_t)found_header + sizeof(heap_header);
        found->header = (heap_header*)new_header;
        found->size = (uint32_t)found_footer - new_header - sizeof(heap_header);
        this->list.best_case_order();

        return (void*)ret;
    }
}

/*Searches free blocks directly before this address, */
heap_header* heap::search_before(heap_header* address)
{
    if (address <= this->start_address)
        return nullptr;						//there is nothing before

#if __CHECKS_ADVNCD
    if (this->start_address + OVERHEAD > address)
        syshlt("HEAP internal error! 2");		//omg you monster, what did you do? at this address shouldnt be a header!
#endif

    heap_footer* found_footer = (uint32_t)address - sizeof(heap_footer);
    if (!found_footer->magic == MAGIC)
        return nullptr;						//no footer here

    heap_header* found_header = found_footer->header;
    if (!found_header->magic == MAGIC)
        syshlt("HEAP internal error! 3");		//there should be a header, but there isn't.

    if (!found_header->is_hole)
        return nullptr;						//nothing to reclaim here

#if __CHECKS_ADVNCD
    if (!this->list.exists(found_header))
        syshlt("HEAP internal error! 16");
#endif
    return found_header;			//also a free block above the freeed one, gonna reclaim that
}

/*Searches free blocks directly after this address, */
heap_header* heap::search_after(heap_footer* address)
{
    if ((address + sizeof(heap_footer)) >= this->end_address)
        return nullptr;						//its the last block in the heap

    heap_header* found_header = (uint32_t)address +sizeof(heap_footer);
    if (found_header->magic != MAGIC) //|| ((heap_footer*)found_header->footer_address)->magic != MAGIC)
        return nullptr;						//nope, no or broken header

    if (!found_header->is_hole)
        return nullptr;

#if __CHECKS_ADVNCD
    if (!this->list.exists(found_header))
        syshlt("HEAP internal error! 17");
#endif
    return found_header;
}

bool heap::expand(uint32_t to)
{
#if __CHECKS_ADVNCD		//should already have been checked
    if (to > this->max_address)
    {
        printfl("to: %u, end: %u", to, this->max_address);
        cli_hlt
//        syshlt("HEAP out of memory! expansion");
    }
#endif

    printf("expanded");
    uint32_t mapped = map_heap(this->end_address, to);
    if (mapped != 0)                                    //could not be mapped, to less memory
    {
        this->contract(mapped);                     //rollback, so unmap the blocks, that could be mapped

        syshlt("HEAP out of memory! expansion 2");
        return false;
    }

    //printfl("Expanded from: %M to: %M", this->end_address - this->start_address, to - this->start_address);
    this->end_address = to;
    return true;
}

void heap::contract(uint32_t until)
{
    if (until < this->min_address)
        until = this->min_address;

    if (until == this->min_address)     //already small enougth
        return;

    uint32_t old = until;
    unmap_heap(this->end_address, until);

    //printfl("Contracted from: %M to: %M", this->end_address - this->start_address - sizeof(heap_header)-sizeof(heap_footer), old - this->start_address - sizeof(heap_header)-sizeof(heap_footer));
    this->end_address = old;
}

uint32_t heap::free(void* ptr)
{
    heap_header* header = (uint32_t)ptr -sizeof(heap_header);

    if (header->magic != MAGIC)
#if __CHECKS_DBG
        syshlt("Cant free at this address");
#else
        return 0;
#endif

    uint32_t tmp_size = header->footer_address - (uint32_t)ptr;
    header->is_hole = true;

    heap_header* found_before = search_before(header);
    heap_header* found_after  = search_after(header->footer_address);

    //### optimize the search to a search_by_size starting from the right end with searching
    //before: _#_  free ___ now connect the end of the 1. with the end of the 3. block
    if (found_before && found_after)	// ___
    {
        //its the block in the memory, with the highest address
        if (found_after->footer_address + sizeof(heap_footer) == this->end_address)
        {
            //but its not the only one, because then we would delete the heap!
            if (found_before != this->start_address)
            {
                list.remove_by_address(found_before);
                list.remove_by_address(found_after);
                header->is_hole = false;

                contract(found_before);

                return header->footer_address - (uint32_t)ptr;
            }
            else	//instead of deleting, we contract him back to min_address
            {
                list.remove_by_address(found_after);

                contract(this->min_address);

                uint32_t footer_new = this->end_address - sizeof(heap_footer);
                install_footer(footer_new, this->start_address);
                found_before->footer_address = footer_new;

                if (list.size != 1)
#if __CHECKS_DBG
                    syshlt("HEAP internal error! 12");
#else
                    return 0;
#endif

                heap_header_info* found_info = &list.data[0];
                found_info->size = this->end_address - this->start_address - sizeof(heap_header)-sizeof(heap_footer);
                //list.best_case_order();

                return tmp_size;
            }
        }

        found_before->footer_address = found_after->footer_address;
        ((heap_footer*)found_after->footer_address)->header = found_before;	//dont forget the footer->header pointer!

        heap_header_info* found_info = list.find_by_address(found_before);

        if (!found_info)
#if __CHECKS_ADVNCD
            syshlt("HEAP internal error! 4");
#else
            return 0;
#endif
        found_info->size = found_after->footer_address - (uint32_t)found_before - sizeof(heap_header);
        found_info->header = found_before;
        list.best_case_order();
        list.remove_by_address(found_after);

        return tmp_size;
    }
    else if (found_before && !found_after)				// __#
    {
        heap_header_info* found_info = list.find_by_address(found_before);

        if (!found_info)
#if __CHECKS_ADVNCD
            syshlt("HEAP internal error! 5");
#else
            return 0;
#endif

        found_before->footer_address = header->footer_address;
        ((heap_footer*)header->footer_address)->header = found_before;

        found_info->header = found_before;

        if (found_before == this->start_address && (found_before->footer_address + sizeof(heap_footer)) == this->end_address && this->end_address > this->min_address)
        {	//the heap is bigger than its minimum size
            contract(this->min_address);

            uint32_t footer_new = this->end_address - sizeof(heap_footer);
            install_footer(footer_new, found_before);
            found_before->footer_address = footer_new;
        }

        found_info->size = found_before->footer_address - (uint32_t)found_before - sizeof(heap_header);
        list.best_case_order();

        return tmp_size;
    }
    else if (!found_before && found_after)				// #__
    {
        heap_header_info* found_info = list.find_by_address(found_after);

        if (!found_info)
#if __CHECKS_ADVNCD
            syshlt("HEAP internal error! 6");
#else
            return 0;
#endif

        header->footer_address = found_after->footer_address;
        ((heap_footer*)found_after->footer_address)->header = header;

        found_info->header = header;

        if (header == this->start_address && this->end_address == (found_after->footer_address + sizeof(heap_footer)) && this->end_address > this->min_address)
        {	//new recaimed block is the only one in the heap, and the heap is bigger than its minimum size
            contract(this->min_address);

            uint32_t footer_new = this->end_address - sizeof(heap_footer);
            install_footer(footer_new, header);
            header->footer_address = footer_new;
        }

        found_info->size = (uint32_t)found_after->footer_address - (uint32_t)header - sizeof(heap_header);
        list.best_case_order();

        return tmp_size;
    }
    else if (header == this->start_address && (header->footer_address +sizeof(heap_footer)) == this->end_address)
    {	//its the only block in memory

        if (this->end_address > this->min_address)  //and its too big
            contract(this->min_address);            //so we contract it

        uint32_t footer_new = this->end_address - sizeof(heap_footer);
        install_footer(footer_new, header);
        header->footer_address = footer_new;

        if (list.size != 0)
#if __CHECKS_DBG
            syshlt("HEAP internal error! 13");
#else
            return 0;
#endif

        heap_header_info new_info;
        new_info.header = header;
        new_info.size = this->end_address - this->start_address - sizeof(heap_header)-sizeof(heap_footer);

        list.add(new_info);
        return tmp_size;
    }
    else								// #_#
    {
        //is_hole is already true.
        heap_header_info new_info;
        new_info.header = header;
        new_info.size = header->footer_address - (uint32_t)header - sizeof(heap_header);

        list.add(new_info);

        return tmp_size;
    }
}

heap::~heap()
{
    //only released on shutdown
    this->min_address = this->max_address = this->start_address = this->end_address = 0;
    list.~ordered_array();
};
}
