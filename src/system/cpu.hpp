#ifndef CPU_HPP
#define CPU_HPP

#include "../types.hpp"

namespace system
{
    #define NM_CR  0x1
    #define NM_CR0 ((NM_CR << 4) | 0x0)
    #define NM_CR4 ((NM_CR << 4) | 0x4)

    #define NM_CPUID 0x2
    #define NM_CPUID_A ((NM_CPUID << 4) | 0x0)
    #define NM_CPUID_B ((NM_CPUID << 4) | 0x1)
    #define NM_CPUID_C ((NM_CPUID << 4) | 0x2)
    #define NM_CPUID_D ((NM_CPUID << 4) | 0x3)

    #define NM_DEF 0x0f

    enum class CPU_CONFIG_OP : uint32_t
    {
        CR0_PROTECTED_MODE                  = (NM_CR0 << 24) |  0,      //PE
        CR0_MONITOR_CO_PROC                 = (NM_CR0 << 24) |  1,      //MP
        CR0_EMULATION                       = (NM_CR0 << 24) |  2,      //EM
        CR0_TASK_SWITCHED                   = (NM_CR0 << 24) |  3,      //TS
        CR0_EXTENSION_TYPE                  = (NM_CR0 << 24) |  4,      //ET
        CR0_NUMERIC_ERROR                   = (NM_CR0 << 24) |  5,      //NE
        CR0_WRITE_PROTECT                   = (NM_CR0 << 24) | 16,      //WP
        CR0_ALIGNMENT_MASK                  = (NM_CR0 << 24) | 18,      //AM
        CR0_NOT_WRITE_THROUGH               = (NM_CR0 << 24) | 29,      //NW
        CR0_CACHE_DISABLED                  = (NM_CR0 << 24) | 30,      //CD
        CR0_PAGING_ENABLED                  = (NM_CR0 << 24) | 31,      //PG

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
        CR4_OS_SUPP_UNMASKED_SIMD_FPU       = (NM_CR4 << 24) | 10,      //OSXMMEXCPT
        CR4_VIRTUAL_MACHINE_EXT             = (NM_CR4 << 24) | 13,      //VMXE
        CR4_SAFER_MODE_EXT_EN               = (NM_CR4 << 24) | 14,      //SMXE
        CR4_FSGSBASE_EN                     = (NM_CR4 << 24) | 16,      //FSGSBASE
        CR4_PCID_EN                         = (NM_CR4 << 24) | 17,      //PCIDE
        CR4_XSAVE_CPU_EXT_EN                = (NM_CR4 << 24) | 18,      //OSXSAVE
        CR4_SUPER_MODE_EXEC_PROTECTED_EN    = (NM_CR4 << 24) | 20,      //SMEP
        CR4_SUPER_MODE_ACCE_PROTECTED_EN    = (NM_CR4 << 24) | 21,      //SMAP
        CR4_PROTECTION_KEY_EN               = (NM_CR4 << 24) | 22,      //PKE

        CPUID_EAX_VENDOR                    = (NM_CPUID_A << 24) |  0,

        CPUID_ECX_SSE3                      = (NM_CPUID_C << 24) |  0,
        CPUID_ECX_PCLMUL                    = (NM_CPUID_C << 24) |  1,
        CPUID_ECX_DTES64                    = (NM_CPUID_C << 24) |  2,
        CPUID_ECX_MONITOR                   = (NM_CPUID_C << 24) |  3,
        CPUID_ECX_DS_CPL                    = (NM_CPUID_C << 24) |  4,
        CPUID_ECX_VMX                       = (NM_CPUID_C << 24) |  5,
        CPUID_ECX_SMX                       = (NM_CPUID_C << 24) |  6,
        CPUID_ECX_EST                       = (NM_CPUID_C << 24) |  7,
        CPUID_ECX_TM2                       = (NM_CPUID_C << 24) |  8,
        CPUID_ECX_SSSE3                     = (NM_CPUID_C << 24) |  9,
        CPUID_ECX_CID                       = (NM_CPUID_C << 24) | 10,
        CPUID_ECX_FMA                       = (NM_CPUID_C << 24) | 12,
        CPUID_ECX_CX16                      = (NM_CPUID_C << 24) | 13,
        CPUID_ECX_ETPRD                     = (NM_CPUID_C << 24) | 14,
        CPUID_ECX_PDCM                      = (NM_CPUID_C << 24) | 15,
        CPUID_ECX_DCA                       = (NM_CPUID_C << 24) | 18,
        CPUID_ECX_SSE4_1                    = (NM_CPUID_C << 24) | 19,
        CPUID_ECX_SSE4_2                    = (NM_CPUID_C << 24) | 20,
        CPUID_ECX_x2APIC                    = (NM_CPUID_C << 24) | 21,
        CPUID_ECX_MOVBE                     = (NM_CPUID_C << 24) | 22,
        CPUID_ECX_POPCNT                    = (NM_CPUID_C << 24) | 23,
        CPUID_ECX_AES                       = (NM_CPUID_C << 24) | 25,
        CPUID_ECX_XSAVE                     = (NM_CPUID_C << 24) | 26,
        CPUID_ECX_OSXSAVE                   = (NM_CPUID_C << 24) | 27,
        CPUID_ECX_AVX                       = (NM_CPUID_C << 24) | 28,

        CPUID_EDX_FPU                       = (NM_CPUID_D << 24) |  0,
        CPUID_EDX_VME                       = (NM_CPUID_D << 24) |  1,
        CPUID_EDX_DE                        = (NM_CPUID_D << 24) |  2,
        CPUID_EDX_PSE                       = (NM_CPUID_D << 24) |  3,
        CPUID_EDX_TSC                       = (NM_CPUID_D << 24) |  4,
        CPUID_EDX_MSR                       = (NM_CPUID_D << 24) |  5,
        CPUID_EDX_PAE                       = (NM_CPUID_D << 24) |  6,
        CPUID_EDX_MCE                       = (NM_CPUID_D << 24) |  7,
        CPUID_EDX_CX8                       = (NM_CPUID_D << 24) |  8,
        CPUID_EDX_APIC                      = (NM_CPUID_D << 24) |  9,
        CPUID_EDX_SEP                       = (NM_CPUID_D << 24) | 11,
        CPUID_EDX_MTRR                      = (NM_CPUID_D << 24) | 12,
        CPUID_EDX_PGE                       = (NM_CPUID_D << 24) | 13,
        CPUID_EDX_MCA                       = (NM_CPUID_D << 24) | 14,
        CPUID_EDX_CMOV                      = (NM_CPUID_D << 24) | 15,
        CPUID_EDX_PAT                       = (NM_CPUID_D << 24) | 16,
        CPUID_EDX_PSE36                     = (NM_CPUID_D << 24) | 17,
        CPUID_EDX_PSN                       = (NM_CPUID_D << 24) | 18,
        CPUID_EDX_CLF                       = (NM_CPUID_D << 24) | 19,
        CPUID_EDX_DTES                      = (NM_CPUID_D << 24) | 21,
        CPUID_EDX_ACPI                      = (NM_CPUID_D << 24) | 22,
        CPUID_EDX_MMX                       = (NM_CPUID_D << 24) | 23,
        CPUID_EDX_FXSR                      = (NM_CPUID_D << 24) | 24,
        CPUID_EDX_SSE                       = (NM_CPUID_D << 24) | 25,
        CPUID_EDX_SSE2                      = (NM_CPUID_D << 24) | 26,
        CPUID_EDX_SS                        = (NM_CPUID_D << 24) | 27,
        CPUID_EDX_HTT                       = (NM_CPUID_D << 24) | 28,
        CPUID_EDX_TM1                       = (NM_CPUID_D << 24) | 29,
        CPUID_EDX_IA64                      = (NM_CPUID_D << 24) | 30,
        CPUID_EDX_PBE                       = (NM_CPUID_D << 24) | 31,

        CPUID_SUPPORTED                     = (NM_DEF << 24) |  0,

        count
    };

    extern char const* const cr0_strings[];
    extern char const* const cr4_strings[];
    extern char const* const cpuid_c_strings[];
    extern char const* const cpuid_d_strings[];

    uint32_t cpu_get_config(CPU_CONFIG_OP op);
    uint32_t cpu_set_config(CPU_CONFIG_OP op, uint32_t v);
    void cpu_dump_all_config();

    uint32_t cpu_get_register_ctrl(uint8_t number);
    void cpu_set_register_ctrl(uint8_t number, uint32_t value);
}

#endif // CPU_HPP
