#include "system.hpp"
#include "cpu.hpp"

extern uint32_t kernel_end;

namespace system
{
    void reboot()
    {
        outb(0x64, 0xFE);
    }

    void sxlb_system_halt(char_t* error_msg)	//__attribute__((noreturn))
    {
        cli
    #if __CHECKS_NLPTR
        if (!error_msg)
            syshlt("Double Fault");     //man wtf, are you trying to troll me?
    #endif

        ui::text::set_fc(FC_LIGHTGRAY | FC_MOD_INTENSITY);
        ui::text::set_bc(BC_LIGHTBLUE);
        ui::text::clear_screen();
        ui::text::set_cursor(0, 0);

        ui::text::writeline("  *****************");
        ui::text::writeline("  *  FATAL ERROR  *");
        ui::text::writeline("  * SYSTEM HALTED *");
        ui::text::writeline("  *****************");

        printfl("\n\nInfo:\n");

        printfl(" Type: ");
        ui::text::set_fc(FC_RED | FC_MOD_INTENSITY);
        printfl("   %s\n", error_msg);
        ui::text::set_fc(FC_LIGHTGRAY | FC_MOD_INTENSITY);

        //printfl(" CPU state:\n");
        //sxlb_system_dumb();

        ui::video::update();

        hlt
        stop    //the OS WILL stop here
    };

    void sxlb_system_dumb()
    {
        uint8_t tab = ui::text::get_tab_with();
        //ui::text::set_tab_with(20);

        ui::text::new_line();
        printfl("   cr0: %u", cpu_get_register_ctrl(0));
        printfl("   cr1: <reserved>");                      //not accessable
        printfl("   cr2: %u", cpu_get_register_ctrl(2));	//faulting address in cr2
        printfl("   cr3: %u", cpu_get_register_ctrl(3));
        printfl("   cr4: %u", cpu_get_register_ctrl(4));

        ui::text::set_tab_with(tab);
    };

    extern "C"
    {
        extern void kernel_start();
    }

    uint32_t kernel_start_address()
    {
        return &kernel_start;
    };

    uint32_t kernel_end_address()
    {
        return &kernel_end;
    };
}
