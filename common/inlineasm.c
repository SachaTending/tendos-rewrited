#include <stdint.h>

unsigned char inb (unsigned short port) 
{
    unsigned char result;
    __asm__("in %%dx, %%al": "=a" (result) : "d" (port));
    return result;
}

void outb (unsigned short port, unsigned char data) 
{
    __asm__("out %%al, %%dx": :"a" (data), "d" (port));
}

unsigned short inw (unsigned short port) 
{
    unsigned short result;
    __asm__("in %%dx, %%ax": "=a" (result) : "d" (port));
    return result;
}

void outw (unsigned short port, unsigned short data) 
{
    __asm__("out %%ax, %%dx": :"a" (data), "d" (port));
}

static inline void out8(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %[value], %[port]" :: [port] "d"(port), [value] "a"(value));
}

static inline void out16(uint16_t port, uint16_t value)
{
    __asm__ volatile("outw %[value], %[port]" :: [port] "d"(port), [value] "a"(value));
}

static inline uint8_t in8(uint16_t port)
{
    uint8_t value;
    __asm__ volatile("inb %[port], %[value]" : [value] "=a"(value) : [port] "d" (port));
    return value;
}

static inline uint32_t x86_get_eflags(void){
    
    uint32_t flags;

    __asm__ volatile(
        "pushfl;"
        "popl %0"
        : "=rm" (flags)
        :: "memory");

    return flags;
}

static inline void x86_set_eflags(uint32_t flags){
    __asm__ volatile(
        "pushl %0;"
        "popfl"
        :: "g" (flags)
        : "memory", "cc");
}

static inline void x86_enable_int(void){
    __asm__ volatile("sti");
}

static inline void x86_disable_int(void){
    __asm__ volatile("cli");
}

static inline void x86_halt(void){
    __asm__ volatile(
		    "cli;"
		    "hlt");
}