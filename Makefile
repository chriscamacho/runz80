# gcc libz80/z80.c main.c -o main
# gcc `pkg-config --cflags gtk+-3.0` -o z80run libz80/z80.c main.c `pkg-config --libs gtk+-3.0` -rdynamic -std=c99

# z88dk/bin/z80asm -b -o=asm/test1.bin asm/test1.asm
LIBS = `pkg-config --libs gtk+-3.0` 
CFLAGS = `pkg-config --cflags gtk+-3.0` -rdynamic -std=c99

all: runz80 asm

runz80: main.c
	gcc $(CFLAGS) -o runz80 libz80/z80.c main.c $(LIBS)

asm: asm/test1.bin asm/test2.bin 

asm/test1.bin: asm/test1.asm
	z88dk/bin/z80asm -l -b -o=$@ $<

asm/test2.bin: asm/test2.asm
	z88dk/bin/z80asm -l -b -o=$@ $<
	
clean:
	rm -f asm/*.bin
	rm -f z80run
