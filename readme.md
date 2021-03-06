This at the moment it is little more that a minimal front end to
libz80, while there are other z80 IDE's I'm developing this for
intellectual exercise. Its main feature will be to have the ability
to use devices provided by plugins.

The idea is that each plugin *instance* will have its own configuration that
will define what memory area (and/or IO ports) its responsible for 

The user will then be able to "build" their own Z80 system by including
and defining which devices (plugins) and how they are attached.

requires gtk3 pkg-config z88dk libz80

http://nightly.z88dk.org/z88dk-latest.tgz
https://github.com/ggambetta/libz80

extract z88dk to this directory
clone libz80 to this directory

![example virtual machine](runz80.png)

./runz80 machine1.xml

will load a machine that will allow you to run hello-input.bin

You should see Hello World! scrolling down the screen if you change the
value of the 8 bit switches, the hex value is displayed in the top
left of the display

example machine config

The following config is using two plugins, simpleIn is used once, and simpleOut is used twice so the
machine has three devices...

the user specifies the first port for the device (the plugin knows how many ports it needs!)

    <machine name="machine1">
        <plugin
            libName="simpleIn"
            label="input switches"
            portStart="0x10" />

        <plugin
            libName="simpleOut"
            label="input value"
            portStart="0x12" />

        <plugin
            libName="simpleOut"
            label="inverted value"
            portStart="0x14" />
    </machine>

having a "machine" like this you might use it with the following z80 code

        org 0
        
    loop:
        in  a,(0x10)    # simpleIn plugin port
        out (0x12),a    # simpleOut port
        cp  0x80
        jr  z, done
        cpl             # invert
        out (0x14),a    # the other simpleOut port
        jr  loop
    done:
        halt


in addition to the simpleIn and simpleOut plugin there is also kBuffer which
provides 1KB of bit pattern monochrome display, giving a luxurious 128x64x1
stunning display ability. In addition to the 1kb of memory mapped interface
(for the display bitmap) there is also a single port used, any value written
to this port will result in the actual display being updated from the memory.
This allows the whole image to be rendered into the memory but only refreshed
once its ready to be displayed.

