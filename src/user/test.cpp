#include "test.hpp"
#include "../io/keyboard.hpp"
#include "../ui/video.hpp"
#include "../ui/textmode.hpp"
#include "../system/syscall.hpp"
#include "../system/system.hpp"

namespace user
{
    //syscall_test
    uint32_t test () { return 564; };
    void syscall_test()
    {
        //((void(*)(const char))system::calls[0])('f');

        //system::syscall(system::call_number::PUTC, 'p', 'h', 'i', 'j', 'k');
       // printf(" n ist: %i", system::syscall(system::call_number::TEST, 0,0,0,0,0));
    }
    //end


    //k_heap test
    const int l = 100;
    LPTR ptr[l];

    bool is_empty()
    {
    }

    void memory_test()
    {
    }
    //end


	void start()
	{
	};
}
