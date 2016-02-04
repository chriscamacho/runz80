    org 0
    

loop:
    in  a,(0x10)
    out (0x12),a
    cp  0x80
    jr  z, done
    cpl
    out (0x14),a

    ld  hl,0x4000
    ld  de,0x400
l2:
    ld  (hl),0x55
    inc hl
    dec de
    ld  a,d
    or  e
    jp  nz,l2

    out (0x20),a    ; any value to port 0x20

    ld  hl,0x4000
    ld  de,0x400
l3:
    ld  (hl),0x33
    inc hl
    dec de
    ld  a,d
    or  e
    jp  nz,l3

    out (0x20),a    ; any value to port 0x20

    
    jr  loop
done:
    halt
dat:
    defb  0

include "asm/font.asm"

