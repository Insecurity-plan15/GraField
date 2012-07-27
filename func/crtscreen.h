#ifndef _FUNC_CRTSCREEN_H
#define _FUNC_CRTSCREEN_H

#include "include/stddef.h"

/* Scrolls the screen */
void crt_ScrollScreen(void);
/* Updates the hardware cursor: the little blinking line
 * on the screen under the last character pressed! */
void crt_UpdateCursor(void);
/* Clears the screen */
void crt_ClearScreen(void);
/* Puts a single character on the screen */
void crt_WriteChar(char c);
/* Uses the above routine to output a string... */
size_t crt_WriteString(const char *text, size_t size);
/* Sets the forecolor and backcolor that we will use */
void crt_SetTextColor(unsigned char forecolor, unsigned char backcolor);
/* Sets our text-mode VGA pointer, then clears the screen for us */
void crt_Init(void);

#endif
