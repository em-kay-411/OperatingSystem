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
; Driver to read from disk
load32:
    mov eax, 1          ; Starting sector from which we want to load from. Sector 0 is the boot sector
    mov ecx, 100        ; Total number of sectors to be loaded
    mov edi, 0x0100000  ; Address of the sector to be loaded
    call ata_lba_read   ; Load the kernel
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax        ; Back up the lba
    ; Send the highest 8 bits of the lba to the hard disk controller
    shr eax, 24         ; Shift eax register 24 bits to the right. Now the eax contains the highest 8 bits
    or eax, 0xE0        ; Selects the masters drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the higest 8 bits of LBA

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finishes sending the total sectors to read

    ; Send few more bits to LBA
    mov eax, ebx        ;Storing the backup lba
    mov dx, 0x1F3
    out dx, al

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx
    shr eax, 8
    out dx, al

    ;Send upper 16 bits
    mov dx, 0x1F5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

.next_sector:
    push ecx

.try_again:
    mov dx, 0x1F7
    in al, dx
    test al, 8
    jz .try_again

    ; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ret


times 510-($ - $$) db 0   ; Fill the remaining space in the bootloader with zeros
dw 0xAA55                ; Boot signature (magic number)
