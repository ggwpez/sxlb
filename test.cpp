#include "test.hpp"
#include "keyboard.hpp"
#include "video.hpp"
#include "textmode.hpp"
#include "syscall.hpp"

namespace user
{
    //syscall_test
    uint32_t test () { return 564; };
    void syscall_test()
    {
        //((void(*)(const char))system::calls[0])('f');

        system::syscall(system::call_number::PUTC, 'p', 'h', 'i', 'j', 'k');
        printf(" n ist: %i", system::syscall(system::call_number::TEST, 0,0,0,0,0));
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
            ptr[i] = memory::k_malloc(_512KiB, 0, nullptr);
        }

        printl("over");
        memory::dump_info(nullptr);
        /*if (hw::keyboard::getc());
        ui::text::clear_screen();

        int v = 1;
        while (!is_empty())
        {
            if (ptr[v % l])
                memory::k_free(ptr[v % l]);

            v *= 4;
        }

        ui::text::clear_screen();
        memory::dump_info(nullptr);*/
    }
    //end


	void start()
	{
        memory_test();
	};
}
