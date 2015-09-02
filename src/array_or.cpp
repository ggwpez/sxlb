#include "array_or.hpp" 
#include "system/system.hpp"
#include "memory/memory.hpp"

ordered_array::ordered_array()
{ }

ordered_array::ordered_array(uint32_t Capacity)
{
	this->element_size = sizeof(heap_header_info);
    this->size = 0;
	this->capacity = Capacity;
	this->data = (heap_header_info*)k_malloc_no_heap(element_size * Capacity, 0, nullptr);	//nullptr error
	
	if (!this->data)
		syshlt("Heap ctor(S, C) error.");
	
    memory::memset(this->data, 0, element_size * Capacity);
}

void ordered_array::best_case_order()	//bubblesort, ### selbst implementieren
{
    int32_t i, j;
	heap_header_info tmp;
	
    if (this->size > 1)
	for (i = size -1; i >= 0; i--)
	{
		for (j = size - 1; j >= 0; j--)
		{
			if (data[j].size < data[j + 1].size)
			{
				tmp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = tmp;
			}
		}
	}
}

void quickSort(heap_header_info arr[], int left, int right)
{
    int i = left, j = right;
    heap_header_info tmp;
    heap_header_info pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j)
    {
        while (arr[i] > pivot)
            i++;
        while (arr[j] < pivot)
            j--;

        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

void ordered_array::worst_case_order()	//quicksort, ### selbst implementieren
{
	quickSort(this->data, 0, this->size - 1);
}

heap_header_info* ordered_array::find_fitting_block(uint32_t size, bool page_aligned, uint32_t* index_out)
{
    int32_t offset = 0;

	if (data[0].size < size)	//all blocks are too small
		return nullptr;

	for (uint32_t i = 1; i < this->size; i++)
	{
        if (page_aligned)
        {
            if (((uint32_t)&data[i]+sizeof(heap_header)) & 0xFFFFF000 != 0)    //should the user-address be page aligned?
                offset = 0x1000 - ((uint32_t)&data[i] +sizeof(heap_header)) % 0x1000;

            int32_t aligned_size = data[i].size - offset;

            if (aligned_size == size)
                return &data[i];
            else if (aligned_size > size)
                *index_out = i;
        }
        else
        {
            if (data[i].size > size)
                *index_out = i;
            else if (data[i].size == size)
                return &data[i];
            else
                break;
        }
	}

    return &data[*index_out];
}

heap_header_info* ordered_array::find_by_address(uint32_t address)
{
	for (uint32_t i = 0; i < this->size; i++)
		if (data[i].header == address)
            return & data[i];

	return nullptr;
}

bool ordered_array::exists(heap_header* address)
{
    for (int32_t i = 0; i < this->size; ++i)
        if (data[i].header == address)
            return true;

    return false;
}

bool ordered_array::remove_by_index(uint32_t index)
{
#if __CHECKS_DBG
    if (!this->size)
    {
        printl("he 1");
        cli_hlt
        syshlt("HEAP empty");
    }
	if (data[index].header == nullptr && index >= this->size)
		syshlt("HEAP remove_by_index error!");
#endif

    data[index].size = 0;
    data[index].header->is_hole = 0;
    data[index].header = nullptr;
	best_case_order();
    this->size--;
	return true;
}

bool ordered_array::remove_by_address(heap_header* address)
{
#if __CHECKS_DBG
    if (!this->size)
    {
        printl("he 2");
        cli_hlt
        syshlt("HEAP empty");
    }
#endif

    address->is_hole = 0;
    for (uint32_t i = 0; i < this->size; i++)
    {
        if (data[i].header == address)
        {
            data[i].size = 0;
            data[i].header = nullptr;
            best_case_order();
            this->size--;
            return true;
        }
    }

#if __CHECKS_DBG
    syshlt("HEAP remove_by_address error!");
#endif
	return false;
}

bool ordered_array::remove_by_address(heap_header_info* address)
{
#if __CHECKS_DBG
    if (!this->size)
    {
        printl("he 3");
        cli_hlt
        syshlt("HEAP empty");
    }
#endif

    address->size = 0;
    address->header->is_hole = 0;
    for (uint32_t i = 0; i < this->size; i++)
    {
        if (&(data[i]) == address)
        {
            data[i].size = 0;
            data[i].header = nullptr;
            best_case_order();
            this->size--;
            return true;
        }
    }

#if __CHECKS_DBG
    syshlt("HEAP remove_by_address error!");
#endif
    return false;
};

bool ordered_array::add(heap_header_info value)
{
   // printfl("added. s: %u, c: %u", value.size, this->size);
    if (this->size >= this->capacity)
        syshlt("HEAP-Table out of memory!");

    this->data[this->size++] = value;
    this->best_case_order();
    value.header->is_hole = true;

    if (!check_size())
    { syshlt("size error"); }

	return true;
}

bool ordered_array::check_size()
{
    int32_t c = 0;
	while (data[c].size != 0 && data[c].header != nullptr) { c++; }

    return (c == this->size);
}

ordered_array::~ordered_array()
{
    if (this->data)
    {
        //memory::k_free(this->data);   //actually should be: k_free_no_heap, but thats unnecessary

        memory::memset(this, 0, sizeof(ordered_array));
    }
}
