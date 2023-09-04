[BITS 32]

extern kernel_main

global _start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG   ; Load the data segment index
    mov ds, ax         ; Set DS to the data segment
    mov fs, ax         ; Set FS to the data segment
    mov es, ax         ; Set ES to the data segment
    mov gs, ax         ; Set GS to the data segment
    mov ss, ax         ; Set SS to the data segment
    mov ebp, 0x00200000; Set the base pointer
    mov esp, ebp       ; Set the stack pointer to the base pointer

    ;Enable the A20 line
    in al, 0x92         ; Read from the bus uses 'in'
    or al, 2
    out 0x92, al        ; Write to the bus

    ;Remap the Master Programmable Interrupt Controller (PIC)
    mov al, 00010001b
    out 0x20, al
    
    mov al, 0x20
    out 0x21, al

    mov al, 00000001b
    out 0x21, al
    sti
    call kernel_main

    jmp $              ; Infinite loop (halt)


times 512-($ - $$) db 0
