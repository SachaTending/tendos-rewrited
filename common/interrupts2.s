section text

global load_idt
global _load_idt

_load_idt:
load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret

global _test2
extern _test
_test2:
    push eax    ;; make sure you don't damage current state             
    cld                   
    call _test
    pop eax  
    mov al,20h
    out 20h,al  ;; acknowledge the interrupt to the PIC              
    iretd                