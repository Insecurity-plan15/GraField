
AS = as
CC = gcc
LD = ld
RM = rm -f
MKDIR = mkdir
INCLUDE = -I.
CFLAGS = -ansi -fno-stack-protector -fno-builtin -nostdinc -Wall -O3 -g3 $(INCLUDE) -m32 $(OPT)
ASMFLAGS = --32 $(ASMOPT)
LDFLAGS = -T link.ld -melf_i386 $(LDOPT)

.PHONY: all clean run debug kernel startup func

all: kernel

clean:
	$(RM) -Rf out
	$(MKDIR) out
	$(MKDIR) out/startup
	$(MKDIR) out/func

package:
	(cd ..; tar cv GraField/ | xz -c9e > GraField-`date +%Y%m%d`.tar.xz)
	@sync

kernel: out/kernel

run: out/kernel
	qemu -kernel out/kernel

debug: out/kernel
	# if you have problems setting breakpoints, use this.
	# qemu -kernel out/kernel -no-kvm -s -S &
	qemu -kernel out/kernel -s -S &
	@sleep 1
	gdb out/kernel -ex 'target remote :1234'

out/kernel: startup func
	$(LD) $(LDFLAGS) -o out/kernel \
	out/startup/kstart.o out/startup/startup.o \
	out/func/memory.o out/func/crtscreen.o


startup: out/startup/kstart.o out/startup/startup.o

out/startup/kstart.o: startup/kstart.S
	$(AS) $(ASMFLAGS) -o $@ $<

out/startup/startup.o: startup/startup.c out/func/memory.o
	$(CC) -c $(CFLAGS) -o $@ $<

startup/startup.c: func/crtscreen.h func/crtscreen.c func/general.h include/multiboot.h


func: out/func/memory.o out/func/crtscreen.o

out/func/memory.o: func/memory.S func/memory.h
	$(AS) $(ASMFLAGS) -o $@ $<

out/func/crtscreen.o: func/crtscreen.c
	$(CC) -c $(CFLAGS) -o $@ $<

func/crtscreen.c: func/crtscreen.h func/ioport.h func/memory.h include/safestr.h include/stddef.h
func/crtscreen.h: include/stddef.h
func/ioport.h: include/stddef.h


include/safestr.h: include/stddef.h
