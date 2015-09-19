#include "string.hpp"

int strcmp (const uint8_t* s1, const uint8_t* s2)
{
  while (*s1 && (*s1 == *s2))
  {
    s1++;
    s2++;
  }

  return (*s1) - (*s2);
}

char* strcpy(char* s1, const char* s2)
{
    char* s = s1;
    while ((*s++ = *s2++) != 0);

    return (s1);
}
