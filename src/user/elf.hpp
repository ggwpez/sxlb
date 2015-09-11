#pragma once

#include "../types.hpp"
#include "../memory/memory.hpp"
#include "../ui/textmode.hpp"

namespace elf
{
    enum class elf_bin_type : uint16_t
    {
        relocatable = 1,
        executable  = 2,
        shared      = 3,
        core        = 4
    };

    enum class elf_bits : ubyte_t
    {
        _32 = 1,
        _64 = 2
    };

    enum class elf_endian : ubyte_t
    {
        little  = 1,
        big     = 2
    };

    enum class elf_version : uint32_t
    {
        original = 1
    };

    enum class elf_cpu : uint16_t     //instruction_set
    {
        M32     = 0x01,
        SPARC   = 0x02,
        i386    = 0x03,
        MIPS    = 0x08,
        PowerPC = 0x14,
        ARM     = 0x28,
        SuperH  = 0x2a,
        IA_64   = 0x32,
        x86_64  = 0x3e,
        AArch64 = 0xb7
    };

    enum class elf_ph_type
    {
        Invalid     = 0,
        Load        = 1,
        Dynamic     = 2,
        Interpreter = 3,
        Note        = 4,
        SH_Lib      = 5,
        PH_Dir      = 6,
        TLS         = 7,
        OS_LOW      = 0x60000000,
        OS_HI       = 0x6fffffff,
        Cpu_LOW     = 0x70000000,
        Cpu_HI      = 0x7fffffff
    };

    //#define ELF_MAGIC 0x7F454C46
    #define ELF_MAGIC 0x464c457f
    struct elf_header_t
    {
        /*uint32_t magic;
        ubyte_t class_;
        ubyte_t data;
        ubyte_t versiob;
        ubyte_t pad;
        ubyte_t nident[9];*/
        ubyte_t magic[16];

        elf_bin_type type;
        elf_cpu cpu;
        elf_version version;

        uint32_t entry;
        uint32_t pht_off;       //program header table offset
        uint32_t sht_off;       //section header table offset

        uint32_t flags;         //architecture dependent
        uint16_t this_size;     //size of this header
        uint16_t pht_entry_s;   //pht entry size
        uint16_t pht_entry_c;   //pht entrys count
        uint16_t sht_entry_s;   //sht entry size
        uint16_t sht_entry_c;   //sht entrys count
        uint16_t shstrndx;
    };

    struct elf_ph_t
    {
        elf_ph_type type;
        uint32_t offset;
        uint32_t v_addr;
        uint32_t p_addr;
        uint32_t size;
        uint32_t mem_s;
        uint32_t flags;
        uint32_t align;
    };

    enum class elf_status_t
    {
        Unknown             = 0,
        Ok                  = 1,
        Magic_err           = 2,
        CPU_err             = 3,
        Type_err            = 4,
        Bits_err            = 5,
        Endiand_err         = 6,
        Version_err         = 7
    };

    //returns the absolute entry address
    LPTR load_file(LPTR address, elf_status_t* result);
}
