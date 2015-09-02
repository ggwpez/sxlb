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
        for (int i = 0; i < l; ++i)
            if (ptr[l])
                return false;
        return true;
    }

    void memory_test()
    {
        memory::memset(ptr, 0, sizeof(ptr));
        for (int i = 0; i < l; ++i)
        {
            ptr[i] = memory::k_malloc(10, 0, nullptr);
        }

        printl("over");
        memory::dump_info(nullptr);
        if (io::keyboard::getc());
        ui::text::clear_screen();

        for (int i = 0; i < l; ++i)
            memory::k_free(ptr[i]);

        if (io::keyboard::getc());
        ui::text::clear_screen();
        memory::dump_info(nullptr);
    }
    //end


	void start()
	{
        memory_test();
	};
}
