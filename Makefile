FILES = "kernel/kmain.c"
LINKDIR = tmp
CCFLAGS = -ffreestanding -O2 -nostdlib -fno-exceptions -c -I include -g -w -m32
LDOPTS := -Tlink.ld -build-id=none -b elf32-i386 -m elf_i386
OUT_FILES = $(LINKDIR)/kernel/loader.o $(LINKDIR)/kernel/kmain.c.o $(LINKDIR)/common/asmpart.o

include Makefile.unix

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
	@echo NASM common/interrupts2.s to $(LINKDIR)/common/interrupts2.o
	@nasm -felf32 common/interrupts2.s -o $(LINKDIR)/common/interrupts2.o
	@echo NASM common/asmpart.s to $(LINKDIR)/common/asmpart.o
	@nasm -felf32 common/asmpart.s -o $(LINKDIR)/common/asmpart.o

run:
	@qemu-system-i386 -hda grub4dos.vhd -drive file=fat:rw:./,format=raw,media=disk -m 2m -serial stdio -d guest_errors -d page -d cpu_reset -d strace -soundhw pcspk -device cirrus-vga


rungdb:
	@qemu-system-i386 -kernel out.kern -m 2m -serial stdio -d guest_errors -d page -d cpu_reset -d strace -s -S
