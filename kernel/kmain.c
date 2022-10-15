#include <stdint.h>
#include <textmode.h>;
#include <multiboot.h>

struct regs
{
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;    
};

void bios32_search(){
    log("BIOS32 Support: Searching BIOS32...\n");
    char *mem = (unsigned char *) 0xE0000;
    while ((unsigned int) mem < 0xFFFFF) {
        if (mem[0] == '_' && mem[1] == '3' && mem[2] == '2' && mem[3] == '_') {
            log("BIOS32 Support: BIOS32 founded at 0x");
            print_hex(mem);
            print_string("\n");
        }
        mem += 16;
    }
}

char* hex32_to_str(char buffer[], unsigned int val)
{
	char const lut[] = "0123456789ABCDEF";
	for (int i = 0; i < 4; i++)
	{
		buffer[i*2+0] = lut[(val >> (28-i*8)) & 0xF];
		buffer[i*2+1] = lut[(val >> (24-i*8)) & 0xF];
	}
	buffer[8] = 0;
	return buffer;
}

char* str_to_char(char buffer[], unsigned char val)
{
	char const lut[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < 4; i++)
	{
		buffer[i*2+0] = lut[(val >> (28-i*8)) & 0xF];
		buffer[i*2+1] = lut[(val >> (24-i*8)) & 0xF];
	}
	return buffer;
}

char* int32_to_str(char buffer[], int val)
{
	char* b = buffer;
	// negation
	if (val < 0) { *b++ = '-'; val *= -1; }
	// move to end of repr.
	int tmp = val;
	do { ++b; tmp /= 10;  } while (tmp);
	*b = 0;
	// move back and insert as we go
	do {
		*--b = '0' + (val % 10);
		val /= 10;
	} while (val);
	return buffer;
}

typedef int size_t;

void print_int(int lol) {
    char buf[1000];
	int32_to_str(buf, lol);
	print_string(buf);
}

void print_hex(lol) {
    char buf[1000];
	hex32_to_str(buf, lol);
	print_string(buf);
}

void print_str(lol) {
  char buf[1000];
	str_to_char(buf, lol);
	print_string(buf);
}

int c = 0;
 
void test3(void)
{
    // unsigned char status = inb(0x64);
    // if (status & 0x01) {
    //     char keycode = inb(0x60);
        // print_string("oh yes");
    // }
    //print_string(frame);
    //print_string("\n");
    print_string("lol\n");
    c++;
    
    // interrupt_end();
}

int i;

void tick()
{
    i++;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

void log(unsigned char *t) {
    print_string("[");
    print_int(i);
    print_string("]");
    print_string(t);
}

void log2(char *t, char *t2) {
    print_string("[");
    print_int(i);
    print_string("]");
    print_string(t);
    print_string(t2);
}

void logln(unsigned char *t) {
    log(t);
    print_string("\n");
}

void kb_handler()
{
    char k = inb(0x60);
    if (k & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
    }
    log("Scan code: ");
    print_int(k);
    print_string("\n");
}

void syscall_handler(struct regs *r)
{
    log("Syscall: triggered\n");
    log("Syscall: EAX: ");
    print_int(r->eax);
    print_string("\n");
}



void kmain(unsigned long addr) {
    clear_screen_vga();
    draw();
    log("TendOS loading now...\n");
    multiboot_info_t *minfo;
    minfo = (multiboot_info_t *) addr;
    log("VM86: ");print_string(detect_v86()? "Yes":"No");printf("\n");
    log("Modules count: ");print_int(minfo->mods_count);print_string("\n");
    log("Draw mode: ");print_int(minfo->framebuffer_type);print_string("\n");
    log("cmdline: ");
    printf(minfo->cmdline);
    print_string("\n");
    log("Loaded by: ");printf(minfo->boot_loader_name);printf("\n");
    multiboot_module_t *mod;
    int i;
    for (i = 0, mod = (multiboot_module_t *) minfo->mods_addr;
           i < minfo->mods_count;
           i++, mod++)
        log("Mod start: 0x");printf(mod->mod_start);printf("\n");
    no_tar(mod->mod_start);
    log("Hello unitialized enviroment!\n");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    // mm_init();
    __asm__ volatile ("sti");
    irq_install_handler(0, tick);
    irq_install_handler(1, kb_handler);
    ata_init();
    irq_install_handler(128, syscall_handler); // trying to look like linux
    // __asm__ volatile ("sti");
    bios32_search();
    syscall_test();
    logln("Ok, im loaded, just type something)");
    while (1) __asm__("hlt");
}