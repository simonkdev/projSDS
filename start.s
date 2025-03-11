;kernel_main label is declared as external because its the name of the main C function of the kernel
.extern kernel_main

;declaring label "start" as global to be accesible outside of this script (for linker.ld)

.global start

;basic info for GRUB (calculating constants and hardcoding them into the executable)

.set MB_MAGIC, 0x1BADB002
;"magic" constant GRUB will use to detect kernel´s location

.set MB_FLAGS, (1 << 0 | 1 << 1)
;loads modules on page boundaries and provides memory map (used later in development)

.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))
;creates a checksum that includes previous values

;here starts the section of the executable that will contain our multiboot header
.section .multiboot
    .align 4
    ;ensures following date is aligned on multiple of 4 bytes
    
    ;use constants and checksum:
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM


;contains data initialised to zeros when kernel is loaded
.section .bss
;the C code needs a stack to run, allocated to 4096 bytes (4kB)
;possible for expansion
.align 16
stack_bottom:
    .skip 4096  ;Reserves a 4kB stack
stack_top:

.section .text
;start label, this is where the first code that gets run in kernel
start:

;set up environment that can run C code. (all we gotta do is set up the stack)
;HINT: the stack on x86 grows DOWN, so we have to start at the top.

mov $stack_top, %esp ;set stack pointer to top of the stack

;now we are ready to run c code:

call kernel_main

;If something goes to shit we gotta hang the CPU

hang:
    cli ;disables CPU interrrupts
    hlt ;halt the CPU
    jmp hang ;"go back if youre here and start over" because if CPU is put on halt it will stop executing
    