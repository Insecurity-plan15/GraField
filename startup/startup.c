#include "func/crtscreen.h"
#include "func/general.h"
#include "include/multiboot.h"
#include "include/safestr.h"

void cstart(multiboot_info_t *bootinfo)
{
    crt_Init();
    crt_WriteString(STRSIZE("Starting GraField OS..."));
    spin();
}

