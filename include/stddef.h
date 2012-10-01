#ifndef _INCLUDE_STDDEF_H
#define _INLCUDE_STDDEF_H

/* Define NULL */
#ifndef NULL
#ifdef __GNUG__
#define NULL __null
#else   /* G++ */
#ifndef __cplusplus
#define NULL ((void *) 0)
#else   /* C++ */
#define NULL 0
#endif  /* C++ */
#endif  /* G++ */
#endif

/* Define size_t */
#ifndef size_t
#if defined(__i386__)
typedef unsigned long int size_t;
#elif defined(__x86_64__) || defined(__amd64__) || defined(_M_X64)
typedef unsigned long long int size_t;
#else
typedef unsigned int size_t;
#endif
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#ifndef inline
#define inline __inline__
#endif
#endif

/* Define useful types like u16, s32 */
typedef signed   char      int s8;
typedef unsigned char      int u8;
typedef signed   short     int s16;
typedef unsigned short     int u16;
typedef signed   long      int s32;
typedef unsigned long      int u32;
typedef signed   long long int s64;
typedef unsigned long long int u64;

#endif
