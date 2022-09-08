#define IDT_SIZE 256

struct idt_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short int offset_higherbits;
};

struct idt_pointer
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt_table[IDT_SIZE];
struct idt_pointer idt_ptr;

void load_idt_entry(char isr_number, unsigned long base, short int selector, char flags)
{
    idt_table[isr_number].offset_lowerbits = base & 0xFFFF;
    idt_table[isr_number].offset_higherbits = (base >> 16) & 0xFFFF;
    idt_table[isr_number].selector = selector;
    idt_table[isr_number].flags = flags;
    idt_table[isr_number].zero = 0;
}

static void initialize_idt_pointer()
{
    idt_ptr.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idt_ptr.base = (unsigned int)&idt_table;
}

void reload_idt()
{
    load_idt(&idt_ptr);
}

void idt_init()
{
    pic_init();
    print_string("PIC Initializated.\n");
    initialize_idt_pointer();
    print_string("IDT Pointer initializated.\n");
    load_idt(&idt_ptr);
    print_string("IDT Loaded.\n");
}

