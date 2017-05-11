; connects to a terminal see simple.xml,

	SerIO:		equ		0x20
	SerAvail:	equ		0x21

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
	
	pop		af
	ld		hl,replyval
	call	printByte

	ld		hl,reply
	call	printString

	ld		hl,dnl
	call	printString


	halt

; blocking get char, returns A
getCh:
	in		a,(SerAvail)
	cp		0
	jr		z, getCh
	in		a,(SerIO)
	ret

; HL start of string must be \0 terminated
printString:
	ld		a,(hl)
	cp		0
	jr		nz,cont
	ret
cont:
	out		(SerIO),a
	inc		hl
	jp		printString

; writes the hexadecimal value in A
; into a string overwriting at HL
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
;	out		(SerIO),a
	ld		(hl),a

	ld		a,d
	and		0xf
	cp		0xa
	jr		c,g92

	add		0x37
	jr		l92
g92:
	add		0x30
l92:
	inc		hl
	ld		(hl),a
	;out		(SerIO),a
	
	ret

nl:			defb	"\r\n\0"
dnl:		defb	"\r\n\r\n\0"
pkeymsg:	defb	"Press a key \0"
reply:		defb	"You pressed ascii 0x"
replyval:	defb	"XX\0"
