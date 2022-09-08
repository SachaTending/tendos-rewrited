#include <textmode.h>;

void bios32_search(){
    print_string("BIOS32 Support: Searching BIOS32...\n");
    char *mem = (unsigned char *) 0xE0000;
    while ((unsigned int) mem < 0xFFFFF) {
        if (mem[0] == '_' && mem[1] == '3' && mem[2] == '2' && mem[3] == '_') {
            print_string("BIOS32 Support: BIOS32 founded!\n");
        }
        mem += 16;
    }
}

void test(void){
    // unsigned char status = inb(0x64);
    // if (status & 0x01) {
    //     char keycode = inb(0x60);
        // print_string("oh yes");
    // }
    // print_string("lol\n");

    // outb(0x20, 0x20);
}

void test2(void);

void kmain(void) {
    print_string("TendOS loading now...\n");
    print_string("Hello unitialized enviroment!\n");
    bios32_search();
    idt_init();
    load_idt_entry(0x21, (unsigned long) test2, 0x08, 0x8e);
    reload_idt();
    // unsigned char curmask_master = inb (0x21);
    outb(0x21, 0xFD);
    while (1) __asm__("hlt\n\t");
}