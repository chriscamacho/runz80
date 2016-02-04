    org 0
    

loop:
    in  a,(0x10)
    out (0x12),a
    cp  0x80
    jr  z, done
    cpl
    out (0x14),a
    jr  loop
done:
    halt
dat:
    defb  0
