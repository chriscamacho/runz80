    .module my_crt0

    .globl _putchar
    .globl _getch
    .globl  _main

    .area   _HEADER (ABS)
    ;; Reset vector
    .org    0
    jp  init

    .org    0x08
    ei
    reti
    .org    0x10
    ei
    reti
    .org    0x18
    ei
    reti
    .org    0x20
    ei
    reti
    .org    0x28
    ei
    reti
    .org    0x30
    ei
    reti
    .org    0x38
    ei
    reti

    .org    0x100
init:
    ;; Set stack pointer directly above top of memory.
    ld  sp,#0x0000

    call 0x8000 ; _start _main ??


_putchar:
    ld a, l
    out (0x20), a
    ret

_getch:
    in a, (0x21)
    cp #0
    ld de, #0x0000

    jr z, _getch
    
    in a, (0x20)
    ld e, a
    ld d, #0

    ret


