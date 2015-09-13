#include "convert.h"
#include "../macro.h"

#ifdef __cplusplus
extern "C" {
#endif

char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
char hex_i[] = { 'f', 'd', 'e', 'c', 'b', 'a', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0' };

void reverse(char* s, int32_t count)
{
    int temp, i;
    for (i = 0; i < (count >> 1); ++i)
    {
        temp = s[count - i - 1];
        s[count - i - 1] = s[i];
        s[i] = temp;
    }
};

/*Buffer length = 2*/
void convert_itoa_u2_hex(int8_t number, char* string)
{
    string[0] = '0' + number;
    string[1] = '\0';
};

/*Buffer length = 2*/
void convert_itoa_u4_hex(int8_t number, char* string)
{
    string[0] = hex[B(00001111) & number];
    string[1] = '\0';
};

/*Buffer length = 3*/
void convert_itoa_s8_hex(int8_t number, char* string)
{
    if (number < 0)
        convert_itoa_u8_hex(0xff + number + 1, string);
    else
        convert_itoa_u8_hex(number, string);
};

/*Buffer length = 3*/
void convert_itoa_u8_hex(uint8_t number, char* string)
{
    if (number == 0)	//0 || -0
    {
        string[0] = '0';
        string[1] = '0';
        string[2] = '\0';
        return;
    }

    string[0] = hex[number >> 4];
    string[1] = hex[B(00001111) & number];

    string[2] = '\0';
};

/*Buffer length = 5*/
void convert_itoa_s16_hex(int16_t number, char* string)
{
    if (number & (1 << 15))
        convert_itoa_u16_hex(0xffff + number + 1, string);
    else
        convert_itoa_u16_hex(number, string);
};

/*Buffer length = 5*/
void convert_itoa_u16_hex(uint16_t number, char* string)
{
    uint8_t i = 0;

    if (number == 0)
    {
        string[i++] = '0';
        string[i] = '\0';
        return;
    }

    if (number <= 0xf)
    {
        string[i++] = hex[number];
    }
    else if (number <= 0xff)
    {
        string[i++] = hex[number >> 4];
        string[i++] = hex[0x0f & number];
    }
    else if (number <= 0xfff)
    {
        string[i++] = hex[number >> 8];
        string[i++] = hex[(0x0f0 & number) >> 4];
        string[i++] = hex[0x00f & number];
    }
    else if (number <= 0xffff)
    {
        string[i++] = hex[number >> 12];
        string[i++] = hex[(0x0f00 & number) >> 8];
        string[i++] = hex[(0x00f0 & number) >> 4];
        string[i++] = hex[0x000f & number];
    }
    string[i] = '\0';
};

/*Buffer length = 5*/
void convert_itoa_s16_dec(int16_t number, char* string)
{
    if (number == 0 || number == (1 << 15))	//0 || -0
    {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    uint8_t sign = (number < 0), i = 0;
    string[i++] = '\0';

    while (number)
    {
        string[i++] = '0' + (sign ? -number % 10 : number % 10);
        number /= 10;
    }
    if (sign)
        string[i++] = '-';

    reverse(string, i);
};

/*Buffer length = 5*/
void convert_itoa_u16_dec(uint16_t number, char* string)
{
    if (number == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    uint8_t i = 0;
    string[i++] = '\0';

    while (number)
    {
        string[i++] = '0' + number % 10;
        number /= 10;
    }

    reverse(string, i);
};

/*Buffer length = 12*/
void convert_itoa_s32_dec(int32_t number, char* string)
{
    if (number == 0 || number == (1 << 31))	//0 || -0
    {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    uint8_t sign = (number < 0), i = 0;
    string[i++] = '\0';

    while (number)
    {
        string[i++] = '0' + (sign ? -number % 10 : number % 10);
        number /= 10;
    }
    if (sign)
        string[i++] = '-';

    reverse(string, i);
};

/*Buffer length = 11*/
void convert_itoa_u32_dec(uint32_t number, char* string)
{
    if (number == 0)
    {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    uint8_t i = 0;
    string[i++] = '\0';

    while (number)
    {
        string[i++] = '0' + number % 10;
        number /= 10;
    }

    reverse(string, i);
};

/*Buffer length = 9*/
void convert_itoa_s32_hex(int32_t number, char* string)
{
    if (number & (1 << 31))
        convert_itoa_u32_hex(0xffffffff + number +1, string);
    else
        convert_itoa_u32_hex(number, string);
};

/*Buffer length = 9*/
void convert_itoa_u32_hex(uint32_t number, char* string)
{
    uint8_t i = 0;

    if (number == 0)
    {
        string[i++] = '0';
        string[i] = '\0';
        return;
    }

    if (number <= 0xf)
    {
        string[i++] = hex[number];
    }
    else if (number <= 0xff)
    {
        string[i++] = hex[number >> 4];
        string[i++] = hex[0x0f & number];
    }
    else if (number <= 0xfff)
    {
        string[i++] = hex[number >> 8];
        string[i++] = hex[(0x0f0 & number) >> 4];
        string[i++] = hex[0x00f & number];
    }
    else if (number <= 0xffff)
    {
        string[i++] = hex[number >> 12];
        string[i++] = hex[(0x0f00 & number) >> 8];
        string[i++] = hex[(0x00f0 & number) >> 4];
        string[i++] = hex[0x000f & number];
    }
    else if (number <= 0xfffff)
    {
        string[i++] = hex[number >> 16];
        string[i++] = hex[(0x0f000 & number) >> 12];
        string[i++] = hex[(0x00f00 & number) >> 8];
        string[i++] = hex[(0x000f0 & number) >> 4];
        string[i++] = hex[ 0x0000f & number];
    }
    else if (number <= 0xffffff)
    {
        string[i++] = hex[number >> 20];
        string[i++] = hex[(0x0f0000 & number) >> 16];
        string[i++] = hex[(0x00f000 & number) >> 12];
        string[i++] = hex[(0x000f00 & number) >> 8];
        string[i++] = hex[(0x0000f0 & number) >> 4];
        string[i++] = hex[ 0x00000f & number];
    }
    else if (number <= 0xfffffff)
    {
        string[i++] = hex[number >> 24];
        string[i++] = hex[(0x0f00000 & number) >> 20];
        string[i++] = hex[(0x00f0000 & number) >> 16];
        string[i++] = hex[(0x000f000 & number) >> 12];
        string[i++] = hex[(0x0000f00 & number) >> 8];
        string[i++] = hex[(0x00000f0 & number) >> 4];
        string[i++] = hex[ 0x000000f & number];
    }
    else
    {
        string[i++] = hex[number >> 28];
        string[i++] = hex[(0x0f000000 & number) >> 24];
        string[i++] = hex[(0x00f00000 & number) >> 20];
        string[i++] = hex[(0x000f0000 & number) >> 16];
        string[i++] = hex[(0x0000f000 & number) >> 12];
        string[i++] = hex[(0x00000f00 & number) >> 8];
        string[i++] = hex[(0x000000f0 & number) >> 4];
        string[i++] = hex[ 0x0000000f & number];
    }
    string[i] = '\0';
};

#ifdef __cplusplus
}
#endif
