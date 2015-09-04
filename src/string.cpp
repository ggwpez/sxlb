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
