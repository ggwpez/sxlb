#include "../types.hpp"

#ifndef _QUEUE_H
#define _QUEUE_H

template<class T, size_t _size>
/*Implementes a basic FIFO queue with fixed size (static allocation).*/
class queue
{
    T buffer[_size];
    size_t start, length;
    public:
    void create()
    {
        start = 0;
        length = 0;
    }

    T & front()
    {
        assert(!empty());
        return buffer[start];
    }

    T const& front() const
    {
        assert(!empty());
        return buffer[start];
    }

    T & pop_front()
    {
        assert(!empty());

        T &ret = buffer[start++];

        if(start == _size)
            start = 0;

        --length;

        return ret;
    }

    bool remove_front(size_t count)
    {
        if (count > length)
            return false;

        start += count;
        length -= count;
        if(start >= _size)
            start-=_size;

        return true;
    }

    size_t size() const { return length; }

    void clear()
    {
        length = 0;
        start = 0;
    }

    bool empty() const
    {
        return !length;
    }
    bool full() const
    {
        return length == _size;
    }

    bool push_back(T const& value)
    {
        if (full())
            return false;

        if(start+length >= _size)
        buffer[start+length-_size] = value;
        else
        buffer[start+length] = value;

        ++length;
        return true;
    }

    T &operator[](size_t i)
    {
        assert(i < length);
        i+=start;
        if(i >= _size)
        i -= _size;
        return buffer[i];
    }

    T const&operator[](size_t i) const
    {
        assert(i < length);
        i+=start;
        if(i >= _size)
        i -= _size;
        return buffer[i];
    }
};

#endif
