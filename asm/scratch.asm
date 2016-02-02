    org 0
    ld  a,0x12
    out (0x2),a
    ld  a,0x34
    out (0x4),a
    ld  a,0x56
    out (0x6),a
    ld a,0x78
    out (0x9a),a
    halt 
dat:
    defb  0
over:
