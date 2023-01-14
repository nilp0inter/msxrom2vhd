# msxrom2vhd

Convert MSX rom files into bootable VHD images for MiSTer.


Dependencies
============

* `libguestfs`
* A C compiler.


Installation
============


From a release
--------------

Download the latest [release](https://github.com/nilp0inter/msxrom2vhd/releases) and run:

```
$ tar -zxvf msxrom2vhd-?.?.tar.gz
$ cd msxrom2vhd-?.?
$ ./configure && make
$ sudo make install
```

Install from flake
------------------

```console
$ nix profile install github:nilp0inter/msxrom2vhd
```

Usage
=====

```
Usage: msxrom2vhd <input.rom> <output.vhd>
```

Run from nix flake
------------------

There is no need to install the package first.

```console
$ nix run github:nilp0inter/msxrom2vhd
```

Development
===========

For development you also need `autotools`.

```
./autoconf.sh && ./configure && make
```

Develop with nix flake
----------------------

```console
$ nix develop
```

Acknowledgements
================

This software contains binary files gathered from https://github.com/MiSTer-devel/MSX_MiSTer/tree/master/Utils/sdcreate.zip as well as `execrom` from https://sourceforge.net/projects/execrom/files/ExecROM%20binaries/1.2c/.

