ORG 0      ; Set the origin to memory address 0
BITS 16    ; Set the assembly mode to 16-bit

jmp 0x7c0:start ; Jump to the 'start' label, which sets the code segment

handle_zero:
    ; Output 'A' to the screen
    mov ah, 0eh
    mov al, 'A'
    mov bx, 0x00
    int 0x10  ; BIOS interrupt for screen output
    iret     ; Return from interrupt

start:
    cli     ; Clear interrupts
    mov ax, 0x7c0
    mov ds, ax  ; Set the data segment to 0x7c0
    mov es, ax  ; Set the extra segment to 0x7c0
    mov ax, 0x00
    mov ss, ax  ; Set the stack segment to 0x00
    mov sp, 0x7c00  ; Set the stack pointer
    sti     ; Enable interrupts

    mov word[ss:0x00], handle_zero  ; Set the first word of the stack to 'handle_zero' address
    mov word[ss:0x02], 0x7c0       ; Set the third word of the stack to 0x7c0

    int 0    ; Software interrupt

    mov si, message  ; Load the address of the 'message' string into SI register
    call print      ; Call the 'print' subroutine
    jmp $           ; Infinite loop (halt)

print:
    mov bx, 0
.loop: 
    lodsb           ; Load the next character from the string into AL and increment SI
    cmp al, 0       ; Compare AL (the current character) to 0 (end of string)
    je .done        ; If it's the end of the string, exit the loop
    call print_char ; Call the 'print_char' subroutine to print the character
    jmp .loop       ; Continue the loop
.done:
    ret             ; Return from the subroutine

print_char:
    mov ah, 0eh     ; Set up AH for screen output
    int 0x10        ; BIOS interrupt for screen output
    ret             ; Return from the subroutine

message: db 'Hello World!', 0  ; Define the 'message' string

times 510-($ - $$) db 0   ; Fill the remaining space in the bootloader with zeros
dw 0xAA55                ; Boot signature (magic number)
