#include "elf.hpp"
namespace elf
{
    elf_status_t check_file(elf_header_t* h)
    {
        if (*(uint32_t*)h->magic != ELF_MAGIC)
            return elf_status_t::Magic_err;
        if (h->cpu != elf_cpu::i386)
            return elf_status_t::CPU_err;
        if (h->type != elf_bin_type::executable)
            return elf_status_t::Type_err;
        /*if (h->bits != elf_bits::_32)
            return elf_status_t::Bits_err;
        if (h->endian != elf_endian::little)
            return elf_status_t::Endiand_err;*/
        if (h->version != elf_version::original)
            return elf_status_t::Version_err;

        return elf_status_t::Ok;
    }

    LPTR load_file(LPTR address, elf_status_t* result)
    {
        elf_header_t* h = address;
        *result = check_file(h);
        if (*result != elf_status_t::Ok)
            return (LPTR)h->cpu;

        return h->entry;
    }
}
