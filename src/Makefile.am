# what flags you want to pass to the C compiler & linker
AM_CFLAGS = --pedantic -Wall -std=c99 -O2 -lguestfs -Wno-pointer-sign
AM_LDFLAGS = 
CLEANFILES = command2com.c msxdos2sys.c nextorsys.c ocmbiosdat.c execromcom.c autoexecbat.c

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = msxrom2vhd
msxrom2vhd_SOURCES = command2com.c msxdos2sys.c nextorsys.c ocmbiosdat.c execromcom.c autoexecbat.c main.c

autoexecbat.c:
	xxd -i static/AUTOEXEC.BAT > autoexecbat.c

command2com.c:
	xxd -i static/COMMAND2.COM > command2com.c

msxdos2sys.c:
	xxd -i static/MSXDOS2.SYS > msxdos2sys.c

nextorsys.c:
	xxd -i static/NEXTOR.SYS > nextorsys.c

ocmbiosdat.c:
	xxd -i static/OCM-BIOS.DAT > ocmbiosdat.c

execromcom.c:
	xxd -i static/execrom.com > execromcom.c
