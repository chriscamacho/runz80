    org     0
    ld      a,15
    ld      hl,0x8000
loop:
    ld      (hl),a
    inc     hl
    dec     a
    jp      nz,loop

    halt
     
