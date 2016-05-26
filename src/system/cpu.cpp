#include "cpu.hpp"
#include "cpu_port.hpp"
#include "../memory/memory.hpp"

extern "C"
{
    bool asm_CPUID_supported();
    void asm_CPUID_get_vendor(LPTR out_str);
    uint32_t asm_CPUID_get_feature(uint8_t reg_n);
}

namespace system
{
    uint8_t buffer[64];

    uint32_t cpu_get_config(CPU_CONFIG_OP op)
    {
        uint8_t nm = (uint32_t)op >> 24;            //get namespace
        uint8_t snm = nm >> 4;                      //get super-namespace

        if (snm == NM_CR)
        {
            uint8_t reg = nm & 0xf;
            uint8_t shift = ((uint32_t)op & B(11111));

            return (cpu_get_register_ctrl(reg) & BIT(shift)) >> shift;
        }
        else if (snm == NM_CPUID)
        {
            if (nm == NM_CPUID_A && op == CPU_CONFIG_OP::CPUID_EAX_VENDOR)
            {
                memory::memset(&buffer, 0, sizeof(buffer));
                asm_CPUID_get_vendor(&buffer);

                return &buffer;
            }
            else if ((nm == NM_CPUID_B) || (nm == NM_CPUID_C) || (nm == NM_CPUID_D))
            {
                uint8_t reg = nm & 0xf;
                uint8_t shift = ((uint32_t)op & B(11111));

                return (asm_CPUID_get_feature(reg) & BIT(shift)) >> shift;
            }
        }
        else if (snm == NM_DEF)
        {
            if (op == CPU_CONFIG_OP::CPUID_SUPPORTED)
                return asm_CPUID_supported();
        }
        return -1;
    }

    uint32_t cpu_set_config(CPU_CONFIG_OP op, uint32_t v)
    {
        uint8_t nm = (uint32_t)op >> 24;              //get namespace
        uint8_t snm = nm >> 4;

        if (snm == NM_CR)
        {
            uint8_t reg = nm & 0xf;
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
                printfl("%s = 0x%x", cr0_strings[i], v);
            }
        }

        for (uint8_t i = 0; i < 23; i++)
        {
            if (cr4_strings[i][0])
            {
                uint32_t v = cpu_get_config((CPU_CONFIG_OP)((NM_CR4 << 24) | i));
                printfl("%s = 0x%x", cr4_strings[i], v);
            }
        }

        for (uint16_t i = 0; i < 29; i++)
        {
            if (cpuid_c_strings[i][0])
            {
                uint32_t v = cpu_get_config((CPU_CONFIG_OP)((NM_CPUID_C << 24) | i));
                printfl("%s = 0x%x", cpuid_c_strings[i], v);
            }
        }

        for (uint16_t i = 0; i < 32; i++)
        {
            if (cpuid_d_strings[i][0])
            {
                uint32_t v = cpu_get_config((CPU_CONFIG_OP)((NM_CPUID_D << 24) | i));
                printfl("%s = 0x%x", cpuid_d_strings[i], v);
            }
        }

        LPTR vendor = cpu_get_config(CPU_CONFIG_OP::CPUID_EAX_VENDOR);
        printfl("Vendor: %s", vendor);
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
        "CR4_OS_SUPP_UNMASKED_SIMD_FPU",
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

    char const* const cpuid_c_strings[] =
    {
        "CPUID_ECX_SSE3",
        "CPUID_ECX_PCLMUL",
        "CPUID_ECX_DTES64",
        "CPUID_ECX_MONITOR",
        "CPUID_ECX_DS_CPL",
        "CPUID_ECX_VMX",
        "CPUID_ECX_SMX",
        "CPUID_ECX_EST",
        "CPUID_ECX_TM2",
        "CPUID_ECX_SSSE3",
        "CPUID_ECX_CID",
        "",
        "CPUID_ECX_FMA",
        "CPUID_ECX_CX16",
        "CPUID_ECX_ETPRD",
        "CPUID_ECX_PDCM",
        "", "",
        "CPUID_ECX_DCA",
        "CPUID_ECX_SSE4_1",
        "CPUID_ECX_SSE4_2",
        "CPUID_ECX_x2APIC",
        "CPUID_ECX_MOVBE",
        "CPUID_ECX_POPCNT",
        "CPUID_ECX_AES",
        "CPUID_ECX_XSAVE",
        "CPUID_ECX_OSXSAVE",
        "CPUID_ECX_AVX"
    };

    char const* const cpuid_d_strings[] =
    {
        "CPUID_EDX_FPU",
        "CPUID_EDX_VME",
        "CPUID_EDX_DE",
        "CPUID_EDX_PSE",
        "CPUID_EDX_TSC",
        "CPUID_EDX_MSR",
        "CPUID_EDX_PAE",
        "CPUID_EDX_MCE",
        "CPUID_EDX_CX8",
        "CPUID_EDX_APIC",
        "",
        "CPUID_EDX_SEP",
        "CPUID_EDX_MTRR",
        "CPUID_EDX_PGE",
        "CPUID_EDX_MCA",
        "CPUID_EDX_CMOV",
        "CPUID_EDX_PAT",
        "CPUID_EDX_PSE36",
        "CPUID_EDX_PSN",
        "CPUID_EDX_CLF",
        "",
        "CPUID_EDX_DTES",
        "CPUID_EDX_ACPI",
        "CPUID_EDX_MMX",
        "CPUID_EDX_FXSR",
        "CPUID_EDX_SSE",
        "CPUID_EDX_SSE2",
        "CPUID_EDX_SS",
        "CPUID_EDX_HTT",
        "CPUID_EDX_TM1",
        "CPUID_EDX_IA64",
        "CPUID_EDX_PBE"
    };
}
