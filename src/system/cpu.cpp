#include "cpu.hpp"
#include "cpu_port.hpp"

namespace system
{
    uint32_t cpu_get_config(CPU_CONFIG_OP op)
    {
        uint8_t nm = (uint32_t)op >> 24;              //get namespace

        if ((nm == NM_CR0) || (nm == NM_CR4))
        {
            uint8_t shift = ((uint32_t)op & B(11111));

            return (cpu_get_register_ctrl(nm) & BIT(shift)) >> shift;
        }
        return -1;
    }

    uint32_t cpu_set_config(CPU_CONFIG_OP op, uint32_t v)
    {
        uint8_t nm = (uint32_t)op >> 24;              //get namespace

        if ((nm == NM_CR0) || (nm == NM_CR4))
        {
            uint8_t shift = ((uint32_t)op & B(11111));
            uint32_t reg_val = cpu_get_register_ctrl(nm);

            reg_val |= BIT(shift);
            cpu_set_register_ctrl(nm, reg_val);

            return cpu_get_config(op);
        }
        return -1;
    }

    void cpu_dump_all_config()
    {
        for (uint8_t i = 0; i < 32; i++)
        {
            if (cr0_strings[i][0])             //CR0 enum name not empty?
            {
                uint32_t v = cpu_get_config((CPU_CONFIG_OP)((NM_CR0 << 24) | i));

                printfl("flag %u: %s = 0x%x", i, cr0_strings[i], v);
            }
        }

        for (uint8_t i = 0; i < 23; i++)
        {
            if (cr4_strings[i][0])
            {
                uint32_t v = cpu_get_config((CPU_CONFIG_OP)((NM_CR4 << 24) | i));

                printfl("flag %u: %s = 0x%x", i, cr4_strings[i], v);
            }
        }
    }

    /*Gets the value from: cr0, cr2, cr3, cr4*/
    uint32_t cpu_get_register_ctrl(uint8_t number)
    {
        uint32_t out = 0;

        switch (number)
        {
            case 0:
                __asm__ __volatile__("mov %%cr0, %0": "=r"(out));
                break;
            //case 1:	//CPU reserved
            //	__asm__ __volatile__("mov %%cr1, %0": "=r"(out));
            //	break;
            case 2:
                __asm__ __volatile__("mov %%cr2, %0": "=r"(out));
                break;
            case 3:
                __asm__ __volatile__("mov %%cr3, %0": "=r"(out));
                break;
            case 4:
                __asm__ __volatile__("mov %%cr4, %0": "=r"(out));
                break;
        }

        return out;
    };

    /*Sets the values for: cr0, cr2, cr3, cr4*/
    void cpu_set_register_ctrl(uint8_t number, uint32_t value)
    {
        switch (number)
        {
            case 0:
                __asm__ __volatile__("mov %0, %%cr0":: "r"(value)); // write cr0
                break;
            /*case 1:
                __asm__ __volatile__("mov %0, %%cr1":: "r"(value)); // write cr1
                break;*/
            case 2:
                __asm__ __volatile__("mov %0, %%cr2":: "r"(value)); // write cr2
                break;
            case 3:
                __asm__ __volatile__("mov %0, %%cr3":: "r"(value)); // write cr3
                break;
            case 4:
                __asm__ __volatile__("mov %0, %%cr4":: "r"(value)); // write cr4
                break;
        }
    };

    char const* const cr0_strings[] =
    {
        "CR0_PROTECTED_MODE",
        "CR0_MONITOR_CO_PROC",
        "CR0_EMULATION",
        "CR0_TASK_SWITCHED",
        "CR0_EXTENSION_TYPE",
        "CR0_NUMERIC_ERROR",
        "","","","","","","","","","",
        "CR0_WRITE_PROTECT",
        "",
        "CR0_ALIGNMENT_MASK",
        "","","","","","","","","","",
        "CR0_NOT_WRITE_THROUGH ",
        "CR0_CACHE_DISABLED",
        "CR0_PAGING_ENABLED"
    };

    char const* const cr4_strings[] =
    {
        "CR4_VM86_EXT",
        "CR4_PROTECTED_VIRTUAL_INT",
        "CR4_TIME_STAMP_DIS",
        "CR4_DEBUG_EXT",
        "CR4_PAGE_SIZE_EXT",
        "CR4_PHYS_ADDR_EXT",
        "CR4_MACHINE_CHECK_EX",
        "CR4_PAGE_GLOBAL_EN",
        "CR4_PERFORMANCE_MONITOR_COUNT_EN",
        "CR4_OS_SUPP_FXSAVE",
        "CR4_OS_SUPP_UNMASKED_SIMD_FPE",
        "","",
        "CR4_VIRTUAL_MACHINE_EXT",
        "CR4_SAFER_MODE_EXT_EN",
        "",
        "CR4_FSGSBASE_EN",
        "CR4_PCID_EN",
        "CR4_XSAVE_CPU_EXT_EN",
        "",
        "CR4_SUPER_MODE_EXEC_PROTECTED_EN",
        "CR4_SUPER_MODE_ACCE_PROTECTED_EN",
        "CR4_PROTECTION_KEY_EN"
    };
}
