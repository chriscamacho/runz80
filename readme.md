This at the moment it is little more that a minimal front end to
libz80, while there are other z80 IDE's I'm developing this for
intellectual exercise. Its main feature will be to have the ability
to use devices provided by plugins.

The idea is that each plugin will have its own configuration that
will define what memory area (and/or IO ports) its responsible for 

The user will then be able to "build" their own Z80 system by including
and defining which devices (plugins) and how they are attached.

requires gtk3 pkg-config z88dk libz80

http://nightly.z88dk.org/z88dk-latest.tgz
https://github.com/ggambetta/libz80

extract z88dk to this directory
clone libz80 to this directory

