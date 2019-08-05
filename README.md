# msxrom2vhd

Convert MSX rom files into autolaunch VHD images for MiSTer.


Installation
============

You need `libguestfs` installed in your system.

```
./autoconf.sh && ./configure && make && sudo make install
```


Usage
=====

```
Usage: msxrom2vhd <input.rom> <output.vhd>
```

Acknowledgements
================

This software contains binary files gathered from https://github.com/MiSTer-devel/MSX_MiSTer/tree/master/Utils/sdcreate.zip as well as `execrom` from https://sourceforge.net/projects/execrom/files/ExecROM%20binaries/1.2c/.

