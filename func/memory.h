#ifndef _FUNC_MEMORY_H
#define _FUNC_MEMORY_H

#include "include/stddef.h"

void extern inline *memcpy(void *dest, const void *src, size_t len)
{
    __asm__ __volatile__ ("cld; rep movsb" : : "D" (dest), "S" (src), "c" (len));
    return dest;
}

void extern inline *memset(void *dest, int value, size_t len)
{
    __asm__ __volatile__ ("cld; rep stosb" : : "D" (dest), "a" ((unsigned char) value), "c" (len));
    return dest;
}

void extern inline *memsetw(void *dest, int value, size_t len)
{
    __asm__ __volatile__ ("cld; rep stosw" : : "D" (dest), "a" ((unsigned short int) value), "c" (len));
    return dest;
}

size_t strlen(const char *str);

#endif
