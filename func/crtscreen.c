#include "func/crtscreen.h"
#include "func/ioport.h"
#include "func/memory.h"
#include "include/safestr.h"
#include "include/stddef.h"

/* These define our textpointer, our background and foreground
*  colors (nTextAttributes), and x and y cursor coordinates */
static unsigned short *pCharAtVideoBuf = (unsigned short *) 0xB8000;
static int nTextAttrib = 0x07;
static int iCursorX = 0, iCursorY = 0;

#define CRT_COLS 80
#define CRT_ROWS 25

/* Scrolls the screen */
void crt_ScrollScreen(void)
{
    /* Row 25 is the end, this means we need to scroll up */
    if(iCursorY >= CRT_ROWS)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        register unsigned int temp = iCursorY - CRT_ROWS + 1;
        memcpy((unsigned char *) pCharAtVideoBuf,
               (const unsigned char *) pCharAtVideoBuf + temp * CRT_COLS * 2,
               (CRT_ROWS - temp) * CRT_COLS * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw(pCharAtVideoBuf + (iCursorY - 1) * CRT_COLS * 2, '\0' | (nTextAttrib << 8), (temp + 1) * CRT_COLS);
        iCursorY = CRT_ROWS;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void crt_UpdateCursor(void)
{
    register unsigned int iCursorAtLinearPos;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    iCursorAtLinearPos = iCursorY * CRT_COLS + iCursorX;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    ioport_outb(0x3D4, 14);
    ioport_outb(0x3D5, iCursorAtLinearPos >> 8);
    ioport_outb(0x3D4, 15);
    ioport_outb(0x3D5, iCursorAtLinearPos);
}

/* Clears the screen */
void crt_ClearScreen(void)
{
    /* Sets the entire screen to spaces in our current
    *  color */
    memsetw(pCharAtVideoBuf, '\0' | (nTextAttrib << 8), CRT_COLS * CRT_ROWS);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    iCursorX = 0;
    iCursorY = 0;
    crt_UpdateCursor();
}

/* Puts a single character on the screen */
void crt_WriteChar(char c)
{
    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(iCursorX != 0) iCursorX--;
    }
    /* Handle a DEL by writing ^H SPACE ^H */
    else if(c == 0x7F)
    {
        crt_WriteChar('\x08');
        crt_WriteChar(' ');
        crt_WriteChar('\x08');
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        iCursorX = (iCursorX + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        iCursorX = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        iCursorX = 0;
        iCursorY++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if((unsigned char) c >= (unsigned char) ' ')
    {
        register unsigned short *where = pCharAtVideoBuf + (iCursorY * CRT_COLS + iCursorX);
        *where = (unsigned char) c | nTextAttrib << 8;       /* Character AND nTextAttributes: color */
        ++iCursorX;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(iCursorX >= CRT_COLS)
    {
        iCursorX = 0;
        ++iCursorY;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    crt_ScrollScreen();
    crt_UpdateCursor();
}

/* Uses the above routine to output a string... */
size_t crt_WriteString(const char *text, size_t size)
{
    register size_t len = GetStringLength(text, size);
    size_t lensaved = len;

    while(len--)
    {
        register unsigned long int tmp = Utf8ToUCS2(text);
        crt_WriteChar(tmp<=0xff ? tmp : '\xfe');
        text = Utf8NextChar(text);
    }
    return lensaved;
}

/* Sets the forecolor and backcolor that we will use */
void crt_SetTextColor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    nTextAttrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void crt_Init(void)
{
    pCharAtVideoBuf = (unsigned short *) 0xB8000;
    crt_ClearScreen();
}
