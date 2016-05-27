#ifndef TIME_HPP
#define TIME_HPP

#include "../types.hpp"

typedef struct
{
    /*
    * the number of seconds after the minute, normally in the range
    * 0 to 59, but can be up to 60 to allow for leap seconds
    */
    uint8_t tm_sec;
    /* the number of minutes after the hour, in the range 0 to 59*/
    uint8_t tm_min;
    /* the number of hours past midnight, in the range 0 to 23 */
    uint8_t tm_hour;
    /* the day of the month, in the range 1 to 31 */
    uint8_t tm_mday;
    /* the number of months since January, in the range 0 to 11 */
    uint8_t tm_mon;
    /* the number of years since 1900 */
    int32_t tm_year;
    /* the number of days since Sunday, in the range 0 to 6 */
    uint8_t tm_wday;
    /* the number of days since January 1, in the range 0 to 365 */
    uint16_t tm_yday;
} tm_t;

#endif // TIME_HPP
