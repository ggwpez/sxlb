#pragma once


#include "types.hpp"
#include "textmode.hpp"
cc
void reverse(char_t* s, int32_t count);

/*Buffer length = 2*/
void sxlb_convert_itoa_u2_hex(int8_t number, char_t* string);

/*Buffer length = 2*/
void sxlb_convert_itoa_u4_hex(int8_t number, char_t* string);

/*Buffer length = 3*/
void sxlb_convert_itoa_s8_hex(int8_t number, char_t* string);
/*Buffer length = 3*/
void sxlb_convert_itoa_u8_hex(uint8_t number, char_t* string);

/*Buffer length = 5*/
void sxlb_convert_itoa_s16_hex(int16_t number, char_t* string);
/*Buffer length = 5*/
void sxlb_convert_itoa_u16_hex(uint16_t number, char_t* string);

/*Buffer length = 5*/
void sxlb_convert_itoa_s16_dec(int16_t number, char_t* string);
/*Buffer length = 5*/
void sxlb_convert_itoa_u16_dec(uint16_t number, char_t* string);

/*Buffer length = 12*/
void sxlb_convert_itoa_s32_dec(int32_t number, char_t* string);
/*Buffer length = 11*/
void sxlb_convert_itoa_u32_dec(uint32_t number, char_t* string);

/*Buffer length = 9*/
void sxlb_convert_itoa_s32_hex(int32_t number, char_t* string);
/*Buffer length = 9*/
void sxlb_convert_itoa_u32_hex(uint32_t number, char_t* string);
kk