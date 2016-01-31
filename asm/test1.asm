    ld      a,0x12
    ld      b,0x23
    add     a,b
    ld      (0x8000),a
    halt
     
