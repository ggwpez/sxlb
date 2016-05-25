#ifndef CPU_HPP
#define CPU_HPP

#include "../types.hpp"

namespace system
{
    #define NM_CR0 0x00
    #define NM_CR4 0x04

    enum class CPU_CONFIG_OP : uint32_t
    {
        CR0_PROTECTED_MODE                  = (NM_CR0 << 24) |  0,      //PE   0
        CR0_MONITOR_CO_PROC                 = (NM_CR0 << 24) |  1,      //MP   1
        CR0_EMULATION                       = (NM_CR0 << 24) |  2,      //EM   2
        CR0_TASK_SWITCHED                   = (NM_CR0 << 24) |  3,      //TS   3
        CR0_EXTENSION_TYPE                  = (NM_CR0 << 24) |  4,      //ET   4
        CR0_NUMERIC_ERROR                   = (NM_CR0 << 24) |  5,      //NE   5
        CR0_WRITE_PROTECT                   = (NM_CR0 << 24) | 16,      //WP  16
        CR0_ALIGNMENT_MASK                  = (NM_CR0 << 24) | 18,      //AM  18
        CR0_NOT_WRITE_THROUGH               = (NM_CR0 << 24) | 29,      //NW  29
        CR0_CACHE_DISABLED                  = (NM_CR0 << 24) | 30,      //CD  30
        CR0_PAGING_ENABLED                  = (NM_CR0 << 24) | 31,      //PG  31

        CR4_VM86_EXT                        = (NM_CR4 << 24) |  0,      //VME
        CR4_PROTECTED_VIRTUAL_INT           = (NM_CR4 << 24) |  1,      //PCI
        CR4_TIME_STAMP_DIS                  = (NM_CR4 << 24) |  2,      //TSD
        CR4_DEBUG_EXT                       = (NM_CR4 << 24) |  3,      //DE
        CR4_PAGE_SIZE_EXT                   = (NM_CR4 << 24) |  4,      //PSE
        CR4_PHYS_ADDR_EXT                   = (NM_CR4 << 24) |  5,      //PAE
        CR4_MACHINE_CHECK_EX                = (NM_CR4 << 24) |  6,      //MCE
        CR4_PAGE_GLOBAL_EN                  = (NM_CR4 << 24) |  7,      //PGE
        CR4_PERFORMANCE_MONITOR_COUNT_EN    = (NM_CR4 << 24) |  8,      //PCE
        CR4_OS_SUPP_FXSAVE                  = (NM_CR4 << 24) |  9,      //OSFXSR
        CR4_OS_SUPP_UNMASKED_SIMD_FPE       = (NM_CR4 << 24) | 10,      //OSXMMEXCPT
        CR4_VIRTUAL_MACHINE_EXT             = (NM_CR4 << 24) | 13,      //VMXE
        CR4_SAFER_MODE_EXT_EN               = (NM_CR4 << 24) | 14,      //SMXE
        CR4_FSGSBASE_EN                     = (NM_CR4 << 24) | 16,      //FSGSBASE
        CR4_PCID_EN                         = (NM_CR4 << 24) | 17,      //PCIDE
        CR4_XSAVE_CPU_EXT_EN                = (NM_CR4 << 24) | 18,      //OSXSAVE
        CR4_SUPER_MODE_EXEC_PROTECTED_EN    = (NM_CR4 << 24) | 20,      //SMEP
        CR4_SUPER_MODE_ACCE_PROTECTED_EN    = (NM_CR4 << 24) | 21,      //SMAP
        CR4_PROTECTION_KEY_EN               = (NM_CR4 << 24) | 22,      //PKE

        VM86_SUPP,
        count
    };

    extern char const* const cr0_strings[];
    extern char const* const cr4_strings[];

    uint32_t cpu_get_config(CPU_CONFIG_OP op);
    /* Returns cpu_get_config(op), so you can check if it got set. */
    uint32_t cpu_set_config(CPU_CONFIG_OP op, uint32_t v);
    void cpu_dump_all_config();

    uint32_t cpu_get_register_ctrl(uint8_t number);
    void cpu_set_register_ctrl(uint8_t number, uint32_t value);
}

#endif // CPU_HPP
