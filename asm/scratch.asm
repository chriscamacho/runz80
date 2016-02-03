    org 0
    ld  a,0x12
    out (0x2),a
    ld  a,0x34
    out (0x4),a


loop:
    in  a,(0x40)
    out (0x06),a
    cp  0x80
    jr  nz, loop
    halt
dat:
    defb  0
over:
