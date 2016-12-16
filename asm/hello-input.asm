    org 0
    
    ld      sp,0x8000       

loop:
    ld      hl,(mptr)       
    ld      a,(hl)          ; get the next character of message
    cp      0
    jr      z,hellodone     ; 0 is end of string
    
    inc     hl
    ld      (mptr),hl       ; increment and store the message pointer

    ld      hl,(cursor)     ; where on screen we're printing
    sub     32              ; character data starts at 32 (space)
    
    call    drawchar        ; draw character A at HL

    inc     hl              ; move the cursor to right
    ld      (cursor),hl     

    jr      loop

hellodone:
    ld      hl,0x4182
    ld      (cursor),hl     ; reset screen position
    ld      hl,message
    ld      (mptr),hl       ; reset message pointer

    ld      hl,(curoff)     ; cursor offset 
    ld      bc,0x10
    add     hl,bc           ; add 16 (1 line) to hl

    ld      a,l
    cp      0x90            ; less than 9th line
    jr      nz,offok
    ld      hl,0x0000       ; reset the offset
offok:
    ld      (curoff),hl
    ld      bc,(cursor)
    add     hl,bc
    ld      (cursor),hl     ; add the offset to the cursor


    in      a,(0x10)        ; get the switch positions
    and     0x0f            ; mask lower 4 bits

    cp      10              ; less than 10 (0-9)
    jr      c,gtnd
    add     7               ; skip to A-F
gtnd:
    add     16              ; 0 is char 16

    ld      hl,0x4001       ; least significant nibble position
    call    drawchar


    in      a,(0x10)
    and     0xf0            ; clear c
    rr      a
    rr      a
    rr      a               ; get the switch positions, mask and
    rr      a               ; rotate into lower nibble

    cp      10
    jr      c,gtnt
    add     7
gtnt:
    add     16

    ld      hl,0x4000
    call    drawchar

    out     (0x20),a        ; any value to port 0x20 tells "screen" to refresh        


                            ; clear buffer
    
    ld      hl,0x4000       ; HL = start address of block

    ld      e,l
    ld      d,h
    inc     de              ; DE = HL + 1

    ld      (hl),0x00       ; initialise first byte of block
    ld      bc,0x3ff        ; BC = length of block in bytes
    ldir                    ; fill memory


    jp      loop            ; :o outside JR range !!!
    




mptr:
    defw    message
message:
    defb    "Hello World!",0
cursor:
    defw    0x4182
curoff:                     ; cursor offset
    defw    0x0

drawchar:                   ; a = char hl=screen address
    push    hl

    ld      d,0             ; de = font address
    ld      e,a

    and     a               ; clear carry
    rl      e
    rl      d       ; *2
    rl      e
    rl      d       ; *2 (*4)
    rl      e
    rl      d       ; *2 (*8)

    ld      hl,font         ; add base font address
    add     hl,de

    push    hl              ; load
    pop     de              ; de with hl
    pop     hl
    push    hl              ; restore hl in reg and stack

    ld      bc,0x10         ; 16 bytes per line
charloop:                   ; unrolled loop
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

    pop     hl              ; restore HL
    ret

include "asm/font.asm"

