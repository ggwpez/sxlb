#ifndef CMOS_HPP
#define CMOS_HPP

#include "../types.hpp"

#define CMOS_ADDRESS    0x70
#define CMOS_DATA       0x71

//offsets for reading specific values
#define CMOS_OFFSET_SECONDS     0x00
#define CMOS_OFFSET_SECONDS_ARL 0x01
#define CMOS_OFFSET_MINUTES     0x02
#define CMOS_OFFSET_MINUTES_ALR 0x03
#define CMOS_OFFSET_HOURS       0x04
#define CMOS_OFFSET_HOURS_ALR   0x05
#define CMOS_OFFSET_DAY_OF_WEAK 0x06
#define CMOS_OFFSET_DAY_OF_MONTH 0x07
#define CMOS_OFFSET_MONTH       0x08
#define CMOS_OFFSET_YEAR        0x09

#define CMOS_OFFSET_CENTURY     0x48
#define CMOS_OFFSET_DATE_ALARM  0x49
#define CMOS_OFFSET_MODELL_NR   0x40
#define CMOS_OFFSET_SERIAL_NR_1 0x41
#define CMOS_OFFSET_SERIAL_NR_2 CMOS_OFFSET_SERIAL_NR_1 +1
#define CMOS_OFFSET_SERIAL_NR_3 CMOS_OFFSET_SERIAL_NR_2 +1
#define CMOS_OFFSET_SERIAL_NR_4 CMOS_OFFSET_SERIAL_NR_3 +1
#define CMOS_OFFSET_SERIAL_NR_5 CMOS_OFFSET_SERIAL_NR_4 +1
#define CMOS_OFFSET_SERIAL_NR_6 CMOS_OFFSET_SERIAL_NR_5 +1

#endif // CMOS_HPP

namespace system
{
    /* these two version wait some time after register selection, so CMOS should
     * have enougth time before reading/writing. */
    uint8_t cmos_read(uint8_t offset);
    void    cmos_write(uint8_t offset, uint8_t calue);
}
