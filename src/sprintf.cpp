#include "sprintf.hpp"

void write_string(char_t* dest, char_t* src, uint32_t* maxl, uint32_t offset)
{
	while ((*(dest+++offset) = *src++) && *maxl) 
		(*maxl)--;
};

void write_char(char_t* dest, char src, uint32_t* maxl, uint32_t offset)
{
	if (*maxl)
	{
		(*maxl)--;
		*(dest + offset) = src;
	}
};

int32_t sprintf_s(char_t* str, uint32_t size, const char_t* format, ...)
{
#if __CHECKS_NLPTR
    NOT_NULL(str);
    NOT_NULL(format);
#endif

	va_list ap;
	va_start(ap, format);
	uint32_t u;
	int32_t tmp_s = size, i = -1;
	char_t buffer[128];

	while (format[++i] && size)
	{
		if (format[i] == '%')
		{
			switch (format[++i])
			{
				case 'u':
				{
					u = va_arg(ap, uint32_t);
					sxlb_convert_itoa_u32_dec(u, buffer);
					write_string(str, buffer, &size, tmp_s - size);
				} break;
				case 'i':
				{
					u = va_arg(ap, int32_t);
					sxlb_convert_itoa_s32_dec(u, buffer);
					write_string(str, buffer, &size, tmp_s - size);
				} break;
				case 'x':
				{
					u = va_arg(ap, uint32_t);
					sxlb_convert_itoa_u32_hex(u, buffer);
					write_string(str, buffer, &size, tmp_s - size);
				} break;
				case 'b':
				{
					u = va_arg(ap, uint32_t);		//cant pass a byte, implicit dword
					sxlb_convert_itoa_u8_hex((uint8_t)(0xff & u), buffer);
					write_string(str, buffer, &size, tmp_s - size);
				} break;
				case 's':
				{
					u = va_arg(ap, LPTR);		//cant pass a byte, implicit dword
					write_string(str, (char_t*)u, &size, tmp_s - size);
				} break;
				default:
					write_char(str, '%', &size, tmp_s - size);
					write_char(str, '?', &size, tmp_s - size);
					break;
			}
		}
		else
			write_char(str, format[i], &size, tmp_s - size);
	}
	va_end(ap);
	str[tmp_s - 1] = '\0';
	return tmp_s - size;
};
