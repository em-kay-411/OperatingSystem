ORG 0x7C00      ; Set the origin to memory address 0x7C00 (standard for bootloaders)
BITS 16         ; Set the assembly mode to 16-bit

; Define constants for GDT offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start   ; Jump over the NOP instruction
    nop               ; No operation (placeholder)

times 33 db 0        ; Pad the bootloader to 512 bytes

start:
    jmp 0: step2      ; Jump to the next step

step2:
    cli               ; Disable interrupts
    mov ax, 0x00      ; Load 0x00 into AX
    mov ds, ax        ; Set the data segment to 0x7C0
    mov es, ax        ; Set the extra segment to 0x7C0
    mov ss, ax        ; Set the stack segment to 0x00
    mov sp, 0x7C00    ; Set the stack pointer
    sti               ; Enable interrupts

.load_protected:
    cli
    lgdt [gdt_descriptor] ; Load the Global Descriptor Table (GDT) descriptor
    mov eax, cr0         ; Load the control register CR0
    or eax, 0x1          ; Set the PE (Protection Enable) bit
    mov cr0, eax         ; Update CR0
    jmp CODE_SEG: load32 ; Jump to the 32-bit code segment

; GDT (Global Descriptor Table) entries
gdt_start:
gdt_null:
    dd 0x00
    dd 0x00

; Offset 0x08
gdt_code:
    dw 0xffff   ; Segment limit (0-15 bits)
    dw 0        ; Base (0-15 bits)
    db 0        ; Base (16-23 bits)
    db 0x9a     ; Access Byte (Code segment, readable)
    db 11001111b; Flags (high 4 bits: granularity and low 4 bits: access rights)
    db 0        ; Base (24-31 bits)

; Offset 0x10
gdt_data:
    dw 0xffff   ; Segment limit (0-15 bits)
    dw 0        ; Base (0-15 bits)
    db 0        ; Base (16-23 bits)
    db 0x92     ; Access Byte (Data segment, writable)
    db 11001111b; Flags (high 4 bits: granularity and low 4 bits: access rights)
    db 0        ; Base (24-31 bits)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of GDT
    dd gdt_start               ; Offset to the GDT

[BITS 32]
load32:
    mov ax, DATA_SEG   ; Load the data segment index
    mov ds, ax         ; Set DS to the data segment
    mov fs, ax         ; Set FS to the data segment
    mov es, ax         ; Set ES to the data segment
    mov gs, ax         ; Set GS to the data segment
    mov ss, ax         ; Set SS to the data segment
    mov ebp, 0x00200000; Set the base pointer
    mov esp, ebp       ; Set the stack pointer to the base pointer
    jmp $              ; Infinite loop (halt)

times 510-($ - $$) db 0   ; Fill the remaining space in the bootloader with zeros
dw 0xAA55                ; Boot signature (magic number)
