#include "array_or.hpp" 
#include "system.hpp"


ordered_array::ordered_array()
{ }

void* memset(void* dest, char val, size_t count)
{
	char* temp = (char*)dest;
	for (; count != 0; count--) *(temp++) = val;
	return dest;
};

ordered_array::ordered_array(uint32_t Size, uint32_t Capacity)
{
	this->element_size = sizeof(heap_header_info);
	this->size = Size;
	this->capacity = Capacity;
	this->data = (heap_header_info*)k_malloc_no_heap(element_size * Capacity, 0, nullptr);	//nullptr error
	
	if (!this->data)
		syshlt("Heap ctor(S, C) error.");
	
	memset(this->data, 0, element_size * Capacity);
}

void ordered_array::best_case_order()	//bubblesort, ### selbst implementieren
{
	int32_t i, j, first = 1;
	heap_header_info tmp;
	
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


heap_header_info* ordered_array::find_fitting_block(uint32_t size, uint32_t* index_out)
{
	uint32_t tmp, best_index = 0;
	if (data[0].size < size)	//all blocks are too small
		return nullptr;

	for (uint32_t i = 1; i < this->size; i++)
	{
		if (data[i].size > size)
		{
			best_index = i;
		}
		else if (data[i].size == size)
			return &data[i];
		else
			break;
	}
	if (!index_out)
		*index_out = best_index;

	return &data[best_index];
}

heap_header_info* ordered_array::find_by_address(uint32_t address)
{
	for (uint32_t i = 0; i < this->size; i++)
	{
		if (data[i].header == address)
			return data + i *sizeof(heap_header_info);
	}
	return nullptr;
}

bool ordered_array::remove_by_index(uint32_t index)
{
#if __CHECKS_DBG
	if (data[index].header == nullptr && index >= this->size)
		syshlt("HEAP remove_by_index error!");
#endif
	//data[index].set_zero();
	data[index].size = 0,
	best_case_order();
	size--;
	return true;
}

bool ordered_array::remove_by_address(heap_header* address)
{
	for (uint32_t i = 0; i < this->size; i++)
	{
		if (data[i].header == address)
		{
			//data[i].set_zero();
			data[i].size = 0;
			best_case_order();
			size--;
			return true;
		}
	}

#if __CHECKS_DBG
	syshlt("HEAP remove_by_address error!");
#endif
	return false;
}

bool ordered_array::add(heap_header_info value)
{
	if (this->size >= this->capacity)
		syshlt("HEAP-Table out of memory!");

	data[size++] = value;
	best_case_order();
	
	return true;
}

bool ordered_array::check_size()
{
	uint32_t c = 0;
	while (data[c].size != 0 && data[c].header != nullptr) { c++; }

	return (c == this->size);
}

ordered_array::~ordered_array()
{
	k_free(this->data);
	this->element_size = 0;
	this->data = NULL;
}
