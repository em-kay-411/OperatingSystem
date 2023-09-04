#ifndef IDT_H
#define IDT_H
#include <stdint.h>

// Structure for the interrupt descripter
struct idt_desc{

    uint16_t offset_1;          // Offset lower bits 0-15
    uint16_t selector;          // Code segment selector
    uint8_t zero;              // Unused 
    uint8_t type_attr;         // Type 
    uint16_t offset_2;          // Offset higher bits 16-31
} __attribute__((packed));

// Structure to point to the insterrupt descriptor array
struct idtr_desc{
    uint16_t limit;             // Length of the IDT array - 1
    uint32_t base;              // Pointer to the IDT array
} __attribute__((packed));

void idt_init();

#endif