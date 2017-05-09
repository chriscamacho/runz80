; connects to a terminal see simple.xml,
; label in main gui show pty to use
; minicom works well

defc	SerIO		=	0x20
defc	SerAvail	=	0x21


	org 0

	ld		sp,0
	
	ld		b,95
	ld		a,0x20
loop:
	out		(SerIO),a
	inc		a
	djnz	loop

	ld		hl,dnl
	call	printString

	ld		hl,pkeymsg
	call	printString

	call	getCh
	push	af

	ld		hl,nl
	call	printString
	
	ld		hl,reply
	call	printString

	pop		af
	call	printByte

	ld		hl,dnl
	call	printString


	halt

; blocking get a char
getCh:
	in		a,(SerAvail)
	cp		0
	jr		z, getCh
	in		a,(SerIO)
	ret


printString:
	ld		a,(hl)
	cp		0
	jr		nz,cont
	ret
cont:
	out		(SerIO),a
	inc		hl
	jp		printString


printByte:
	ld		d,a
	srl		a
	srl		a
	srl		a
	srl		a
	cp		0xa
	jr		c,g91

	add		0x37
	jr		l91
g91:
	add		0x30
l91:
	out		(SerIO),a

	ld		a,d
	and		0xf
	cp		0xa
	jr		c,g92

	add		0x37
	jr		l92
g92:
	add		0x30
l92:
	out		(SerIO),a
	
	ret

nl:			defb	"\r\n\0"
dnl:		defb	"\r\n\r\n\0"
pkeymsg:	defb	"Press a key \0"
reply:		defb	"You pressed ascii 0x\0"
