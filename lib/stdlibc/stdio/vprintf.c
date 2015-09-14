#include "stdio.h"
#include "convert.h"

#ifdef __cplusplus
extern "C" {
#endif

#define print(str) i = 0; while (str[i]) putchar(str[i++]); i

int vprintf(const char* format, va_list arg)
{
    char buffer[64]; // Larger is not needed at the moment

    int pos = 0, i;
    for (; *format; format++)
    {
        switch (*format)
        {
            case '%':
                switch (*(++format))
                {
                    case 'u':
                        convert_itoa_u32_dec(va_arg(arg, uint32_t), buffer);
                        pos += print(buffer);
                        break;
                    case 'i': case 'd':
                        convert_itoa_s32_dec(va_arg(arg, int32_t), buffer);
                        pos += print(buffer);
                        break;
                    case 'x': case 'X':
                        convert_itoa_u32_hex(va_arg(arg, uint32_t), buffer);
                        pos += print(buffer);
                        break;
                    case 's':
                    {
                        char* str = va_arg(arg, char*);
                        pos += print(str);
                        break;
                    }
                    case 'c':
                        putchar((int8_t)va_arg(arg, int32_t));
                        pos++;
                        break;
                    case '%':
                        putchar('%');
                        pos++;
                        break;
                    default:
                        --format;
                        --pos;
                        break;
                    }
                    break;
                default:
                    putchar(*format);
                    pos++;
                    break;
            }
    }
    return (pos);
}

#ifdef __cplusplus
}
#endif
