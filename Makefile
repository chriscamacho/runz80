ASM = z80asm-1.8/z80asm
LIBS = `pkg-config --libs expat gtk+-3.0 gmodule-2.0` -ldl
CFLAGS = -g `pkg-config --cflags expat gtk+-3.0 gmodule-2.0` -rdynamic -std=c99
ASMFILES = $(wildcard asm/*.asm)


all: runz80 plugins z80asm-1.8/z80asm asm

runz80: main.c plugin.c
	gcc $(CFLAGS) -o runz80 libz80/z80.c main.c plugin.c $(LIBS)

#asm: asm/monitor.bin asm/test4.bin asm/test1.bin asm/test2.bin asm/scratch.bin asm/hello-input.bin asm/breakout.bin
asm: $(ASMFILES:.asm=.bin)

asm/%.bin: asm/%.asm
	z80asm-1.8/z80asm -i $< -o $@
	
z80asm-1.8/z80asm:
	gcc -DVERSION=\"1.8\" z80asm-1.8/z80asm.c z80asm-1.8/expressions.c -o z80asm-1.8/z80asm


plugins: plugins/libterminal.so plugins/libsimpleOut.so plugins/libsimpleIn.so plugins/libkBuffer.so plugins/libkeyjoy.so

plugins/%.so: plugSrc/%.c
	gcc -o $@ -fPIC -shared $(CFLAGS) $< $(LIBS)

	
clean:
	rm -f asm/*.bin
	rm -f asm/*.err
	rm -f runz80
	rm -f plugins/*.so

