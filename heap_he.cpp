#include "heap_he.hpp"

bool heap_header_info::operator > (const heap_header_info& other) const
{
	return (this->size > other.size);
};

bool heap_header_info::operator < (const heap_header_info& other) const
{
	return (this->size < other.size);
};

bool heap_header_info::operator ==(const heap_header_info& other) const
{
	return (this->size == other.size);
}

void heap_header_info::set_zero()
{
	this->header = nullptr;
	this->size = 0;
}