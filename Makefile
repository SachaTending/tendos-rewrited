FILES = "kernel/kmain.c"
LINKDIR = tmp
CCFLAGS = -std=gnu99 -ffreestanding -O2 -w -nostdlib -fno-exceptions -c -I include
LDOPTS := -Tlink.ld -build-id=none -b pei-i386
OUT_FILES = $(LINKDIR)/kernel/loader.o $(LINKDIR)/kernel/kmain.c.o

ifeq ($(OS),Windows_NT) 
	include Makefile.win-nt
else 
	ifeq ($(shell uname),Linux)
		include Makefile.unix
	endif
endif

# include part
include drivers/Makefile
include common/Makefile

# build part     

build: loader $(FILES)
	@echo Linking...


$(FILES):
	@echo CC $@ to $(LINKDIR)/$@.o
	@gcc $(CCFLAGS) $@ -o $(LINKDIR)/$@.o

loader:
	@echo NASM kernel/loader.asm to $(LINKDIR)/kernel/loader.o
	@nasm -felf32 kernel/loader.asm -o $(LINKDIR)/kernel/loader.o

run:
	@qemu-system-i386 -kernel out.kern -m 2m -serial stdio -d guest_errors -d page -d cpu_reset -d strace -soundhw pcspk


rungdb:
	@qemu-system-i386 -kernel out.kern -m 2m -serial stdio -d guest_errors -d page -d cpu_reset -d strace -s -S