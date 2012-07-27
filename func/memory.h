#ifndef _FUNC_MEMORY_H
#define _FUNC_MEMORY_H

#include "include/stddef.h"

void extern inline *memcpy(void *dest, const void *src, size_t len)
{
    register void *_dest=dest;
    if(src<dest && src+len>dest)
        __asm__ __volatile__ ("std; rep movsb" : "=D" (dest), "=S" (src), "=c" (len) : "D" (dest+len-1), "S" (src+len-1), "c" (len): "memory", "cc");
    else
        __asm__ __volatile__ ("cld; rep movsb" : "=D" (dest), "=S" (src), "=c" (len) : "D" (dest), "S" (src), "c" (len) : "memory", "cc");
    return _dest;
}

void extern inline *memset(void *dest, int value, size_t len)
{
    register void *_dest=dest;
    __asm__ __volatile__ ("cld; rep stosb" : "=D" (dest), "=c" (len) : "D" (dest), "a" ((unsigned char) value), "c" (len) : "memory", "cc");
    return _dest;
}

void extern inline *memsetw(void *dest, int value, size_t len)
{
    register void *_dest=dest;
    __asm__ __volatile__ ("cld; rep stosw" : "=D" (dest), "=c" (len) : "D" (dest), "a" ((unsigned short int) value), "c" (len) : "memory", "cc");
    return _dest;
}

size_t strlen(const char *str);

#endif
