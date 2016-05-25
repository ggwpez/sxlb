#include "cmos.hpp"
#include "cpu_port.hpp"

namespace system
{
    uint8_t cmos_read(uint8_t offset)
    {
        outb(CMOS_ADDRESS, (1 << 7) | (offset & B(01111111)));     //select register
        /*uint32_t i = 0;
        while (i++ < 50000);                //CMOS is not the fastest TODO: see below*/

        return inb(CMOS_DATA);
    };

    void cmos_write(uint8_t offset, uint8_t value)
    {
        outb(CMOS_ADDRESS, (1 << 7) | (offset & B(01111111)));     //select register
        /*uint32_t i = 0;
        while (i++ < 50000); TODO: switch on*/

        outb(CMOS_DATA, value);
    };
}
