#include <textmode.h>;

void bios32_search(){
    print_string("BIOS32 Support: Searching BIOS32...\n");
    char *mem = (unsigned char *) 0xE0000;
    if (mem[0] == '_' && mem[1] == '3' && mem[2] == '2' && mem[3] == '_'){
        print_string("BIOS32 Support: BIOS32 founded!\n");
    }
    while ((unsigned int) mem < 0xFFFFF) {
        if (mem[0] == '_' && mem[1] == '3' && mem[2] == '2' && mem[3] == '_') {
            print_string("BIOS32 Support: BIOS32 founded!\n");
        }
        mem += 16;
    }
}

void kmain(void) {
    print_string("TendOS loading now...\n");
    print_string("Hello unitialized enviroment!\n");
    bios32_search();
}