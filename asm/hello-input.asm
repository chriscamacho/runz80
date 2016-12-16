    org 0
    
    ld      sp,0x8000

loop:
    ld      hl,(mptr)
    ld      a,(hl)
    cp      0
    jr      z,hellodone
    
    ld      hl,(mptr)
    inc     hl
    ld      (mptr),hl

    ld      hl,(cursor)
    sub     32
    
    call    drawchar

    ld      hl,(cursor)
    inc     hl
    ld      (cursor),hl

    jr      loop

hellodone:
    ld      hl,0x4182
    ld      (cursor),hl ; reset hello world position
    ld      hl,message
    ld      (mptr),hl   ; reset message pointer

    ld      hl,(curoff)
    ld      bc,0x10
    add     hl,bc           ; add 16 (1 line) to hl

    ld      a,l
    cp      0x90            ; 9th line
    jr      nz,offok
    ld      hl,0x0000
offok:
    ld      (curoff),hl
    ld      bc,(cursor)
    add     hl,bc
    ld      (cursor),hl


; clear buffer
    
    ld      hl,0x4000       ; HL = start address of block

    ld      e,l
    ld      d,h
    inc     de              ; DE = HL + 1

    ld      (hl),0x00       ; initialise first byte of block
    ld      bc,0x3ff        ; BC = length of block in bytes
    ldir                    ; fill memory


    in      a,(0x10)
    and     0x0f

    cp      10
    jr      c,gtnd
    add     7
gtnd:
    add     16

    ld      hl,0x4001
    call    drawchar


    in      a,(0x10)
    and     0xf0
    rr      a
    rr      a
    rr      a
    rr      a

    cp      10
    jr      c,gtnt
    add     7
gtnt:
    add     16

    ld      hl,0x4000
    call    drawchar

        

    jp      loop    ; :-o outside JR range !!!
    


done:
    out     (0x20),a    ; any value to port 0x20
    halt

mptr:
    defw    message
message:
    defb    "Hello World!",0
cursor:
    defw    0x4182
curoff:                 ; cursor offset
    defw    0x0,0x0,0x0

drawchar:       ; a = char hl=screen address
    push    hl

    ld      d,0     ; de = font address
    ld      e,a

    and     a       ; clear carry
    rl      e
    rl      d       ; *2
    rl      e
    rl      d       ; *2
    rl      e
    rl      d       ; *2

    ld      hl,font
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

