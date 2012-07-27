#ifndef _INCLUDE_SAFESTR_H
#define _INCLUDE_SAFESTR_H

#include "include/stddef.h"

typedef struct {
    char   *buf;
    size_t  size;
    size_t  bufsize;
} safe_string;

/* Macro used to declare a safe_string */
#define Str2SafeString(_Str) ( \
        (_Str), \
        sizeof (_Str)/sizeof (_Str)[0]-1, \
        sizeof (_Str)/sizeof (_Str)[0] \
)
#define STR STR2SAFESTRING

/* Macro used to use a safe_string as argument requires string's size */
#define SafeString2Arg(_Str) (_Str).buf, (_Str).size
#define STRARG SafeString2Arg

/* Macro used to use a char * as argument requires string's size */
#define StrWithSize(_Str) (_Str), (sizeof (_Str)/sizeof (_Str)[0]-1)
#define STRSIZE StrWithSize

/* Get how many bytes that the string used */
extern inline size_t GetStringSize(safe_string str)
{
    return str.size;
}

/* Safe-string to char *
   similar as the one in PASCAL, but useful */
extern inline char *PChar(safe_string str)
{
    return str.buf;
}

/* Get how many (utf-8) characters the string used,
   poor algorithm, but simple */
extern inline size_t GetStringLength(const char *pstr, size_t size)
{
    register size_t res = 0;

    while(size--)
        if((*(pstr++)&(char) 0xc0)!=(char) 0x80)
            ++res;
    return res;
}

extern inline char *Utf8NextChar(const char *pstr)
{
    ++pstr;
    while((*pstr & (char) 0xc0)==(char) 0x80)
        ++pstr;
    return (char *) pstr;
}

extern inline char *Utf8StringAt(const char *pstr, size_t index)
{
    while(index--)
        pstr = Utf8NextChar(pstr);
    return (char *) pstr;
}

extern inline char Utf8CharAt(const char *pstr, size_t index)
{
    return *Utf8StringAt(pstr, index);
}

extern inline size_t Utf8CharLen(const char *pchar)
{
    if((signed char) *pchar>=0)
        return 1;
    else
    {
        register unsigned short int __res;
        __asm__ __volatile__ (
                "bsfw %1, %0;\n" :
                "=r"(__res) :
                "g"((unsigned short int) ~(unsigned char) *pchar)
        );
        return 15-__res;
    }
}

extern inline unsigned long int Utf8ToUCS2(const char *pchar)
{
    register size_t len = Utf8CharLen(pchar);
    if(len==1)
        return *pchar & 0x7f;
    else
    {
        register unsigned long int __res = *pchar & (0x7f>>len);
        while((*++pchar & 0xc0)==0x80)
            __res = (__res<<6) | (*pchar & 0x3f);
        return __res;
    }
}

#define Utf8ToUtf16 Utf8ToUCS2

#endif
