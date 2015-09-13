#include "stdio.h"
#include "convert.h"

#ifdef __cplusplus
extern "C" {
#endif

int vprintf(const char* args, va_list ap)
{
    uint32_t u;
    int32_t i = -1;

    while (args[++i])
    {
        if (args[i] == '%')
        {
            char buffer[20];
            switch (args[++i])
            {
                case 'u':
                {
                    u = va_arg(ap, uint32_t);
                    convert_itoa_u32_dec(u, buffer);

                    for (int i = 0; buffer[i]; ++i)
                        putchar(buffer[i]);
                } break;
                case 'i':
                {
                    u = va_arg(ap, int32_t);
                    convert_itoa_s32_dec(u, buffer);

                    for (int i = 0; buffer[i]; ++i)
                        putchar(buffer[i]);
                } break;
                case 'x':
                {
                    u = va_arg(ap, uint32_t);
                    convert_itoa_u32_hex(u, buffer);

                    for (int i = 0; buffer[i]; ++i)
                        putchar(buffer[i]);
                } break;
                case 'b':
                {
                    u = va_arg(ap, uint32_t);		//cant pass a byte, implicit dword
                    convert_itoa_u8_hex((uint8_t)(0xff & u), buffer);

                    for (int i = 0; buffer[i]; ++i)
                        putchar(buffer[i]);         //cant use puts, because it append a new line -_-
                } break;
                case 's':
                {
                    u = va_arg(ap, LPTR);			//cant pass a byte, implicit dword
                    puts((char*)u);              //actuallisy is char_t*
                } break;
                default:
                    putchar('%');
                    putchar('?');
                    break;
            }
        }
        else
            putchar(args[i]);
    }
}

#ifdef __cplusplus
}
#endif
