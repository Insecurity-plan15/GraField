#ifndef _FUNC_IOPORT_H
#define _FUNC_IOPORT_H

#include "include/stddef.h"

unsigned char inline ioport_inb(register unsigned short port)
{
    register unsigned char _res;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (_res) : "dN" (port));
    return _res;
}

void inline ioport_outb(register unsigned short port, register unsigned char data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

#endif
