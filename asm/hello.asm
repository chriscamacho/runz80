    org 0
    
    ld      sp,0x8000

loop:
    ld      hl,(mptr)
    ld      a,(hl)
    cp      0
    jr      z,done
    
    ld      hl,(mptr)
    inc     hl
    ld      (mptr),hl
    

    ld      hl,(cursor)
    sub     16


    call    drawchar

    ld      hl,(cursor)
    inc     hl
    ld      (cursor),hl

    jr      loop

done:
    out     (0x20),a    ; any value to port 0x20
    halt

mptr:
    defw    message
message:
    defb    "Hello World!",0
cursor:
    defw    0x4182

drawchar:       ; a = char hl=screen address
    push    hl
    ld      b,8     ; counter

    ld      d,0     ; de = font address
    ld      e,a

    dec     e

    and     a       ; clear carry
    rl      e
    rl      d       ; *2
    and     a       ; clear carry
    rl      e
    rl      d       ; *2
    and     a       ; clear carry
    rl      e
    rl      d       ; *2
    and     a       ; clear carry


    ld      hl,1
    add     hl,de

    push    hl      ; load
    pop     de      ; de with hl
    pop     hl
    push    hl      ; restore hl in reg and stack

    ld      bc,0x10
charloop:
    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a
    inc     de
    add     hl,bc

    ld      a,(de)
    ld      (hl),a

    pop     hl
    ret

include "asm/font.asm"

