#ifndef _CONVERT_H
#define _CONVERT_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void reverse(char* s, int32_t count);

/*Buffer length = 2*/
void convert_itoa_u2_hex(int8_t number, char* string);

/*Buffer length = 2*/
void convert_itoa_u4_hex(int8_t number, char* string);

/*Buffer length = 3*/
void convert_itoa_s8_hex(int8_t number, char* string);
/*Buffer length = 3*/
void convert_itoa_u8_hex(uint8_t number, char* string);

/*Buffer length = 5*/
void convert_itoa_s16_hex(int16_t number, char* string);
/*Buffer length = 5*/
void convert_itoa_u16_hex(uint16_t number, char* string);

/*Buffer length = 5*/
void convert_itoa_s16_dec(int16_t number, char* string);
/*Buffer length = 5*/
void convert_itoa_u16_dec(uint16_t number, char* string);

/*Buffer length = 12*/
void convert_itoa_s32_dec(int32_t number, char* string);
/*Buffer length = 11*/
void convert_itoa_u32_dec(uint32_t number, char* string);

/*Buffer length = 9*/
void convert_itoa_s32_hex(int32_t number, char* string);
/*Buffer length = 9*/
void convert_itoa_u32_hex(uint32_t number, char* string);

#ifdef __cplusplus
}
#endif
#endif
