
LIBS = `pkg-config --libs expat gtk+-3.0 gmodule-2.0` -ldl
CFLAGS = -g `pkg-config --cflags expat gtk+-3.0 gmodule-2.0` -rdynamic -std=c99

all: runz80 asm plugins

runz80: main.c plugin.c
	gcc $(CFLAGS) -o runz80 libz80/z80.c main.c plugin.c $(LIBS)

asm: asm/test4.bin asm/yarc-test5.bin asm/serialOutTest.bin asm/test1.bin asm/test2.bin asm/scratch.bin asm/hello-input.bin asm/breakout.bin


asm/%.bin: asm/%.asm
	z88dk/bin/z80asm -l -b -o=$@ $<

plugins: plugins/libterminal.so plugins/libsimpleOut.so plugins/libsimpleIn.so plugins/libkBuffer.so plugins/libkeyjoy.so

plugins/%.so: plugSrc/%.c
	gcc -o $@ -fPIC -shared $(CFLAGS) $< $(LIBS)

	
clean:
	rm -f asm/*.bin
	rm -f asm/*.lst
	rm -f asm/*.map
	rm -f asm/*.obj
	rm -f asm/*.reloc
	rm -f asm/*.sym
	rm -f runz80
	rm -f plugins/*.so

