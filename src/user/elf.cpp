#include "elf.hpp"

namespace elf
{    
    status_t check_file(header_t* h)
    {
        if (*(uint32_t*)h->magic != MAGIC)
            return status_t::Magic_err;
        if (h->cpu != cpu::i386)
            return status_t::CPU_err;
        if (h->type != bin_type::executable)
            return status_t::Type_err;
        /*if (h->bits != bits::_32)
            return status_t::Bits_err;
        if (h->endian != endian::little)
            return status_t::Endiand_err;*/
        if (h->version != version::original)
            return status_t::Version_err;

        return status_t::Ok;
    }

    LPTR load_file(LPTR address, status_t* result)
    {
        header_t* h = address;
        *result = check_file(h);
        if (*result != status_t::Ok)
            return (LPTR)h->cpu;

        ph_t* ph = nullptr;

        for (int i = 0; i < h->pht_entry_c; ++i)
        {
            ph = (uint32_t)h + h->pht_off + i*h->pht_entry_s;

            if (ph->type != ph_type::Load)
                continue;

            memory::memcpy(ph->p_addr, (uint32_t)h +ph->offset, ph->size);
        }

        return h->entry;
    }
}
