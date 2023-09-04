#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idt_desc idt_descriptors[TOTAL_INTERRUPTS]; // Array of interrupt descriptors
struct idtr_desc idtr_descriptor;                  // IDTR

extern void idt_load(struct idtr_desc* ptr);                   // Importing the idt_load function from .asm
extern void int21h();
extern void no_interrupt();

void int21h_handler(){
    print("Keyboard Pressed\n");
    outb(0x20, 0x20);
}

void idt_zero(){
    print("Divide by zero error\n");
}

void no_interrupt_handler(){
    outb(0x20, 0x20);
}

void idt_set(int interrupt_no, void * address){
    struct idt_desc * interrupt = &idt_descriptors[interrupt_no];
    interrupt->offset_1 = (uint32_t) address & 0x0000ffff;   // Lower bits
    interrupt->selector = KERNEL_CODE_SELECTOR;
    interrupt->zero = 0x00;
    interrupt->type_attr = 0xEE;                            // All the type attributes are combined together for this value
    interrupt->offset_2 = (uint32_t) address >> 16;         // Higher bits

}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors)); // Set all to NULL
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1; // Set limit of IDTR
    idtr_descriptor.base = (uint32_t) idt_descriptors;              // Set pointer to the array for IDTR

    for(int i=0; i<TOTAL_INTERRUPTS; i++){
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    // Load the IDT
    idt_load(&idtr_descriptor);
}