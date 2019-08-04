ALL: msxrom2vhd

clean:
	-rm -f autoexecbat.h command2com.h msxdos2.h nextorsys.h ocmbiosdat.h execromcom.h
	-rm -f msxrom2vhd

autoexecbat.h:
	xxd -i static/AUTOEXEC.BAT > autoexecbat.h

command2com.h:
	xxd -i static/COMMAND2.COM > command2com.h

msxdos2sys.h:
	xxd -i static/MSXDOS2.SYS > msxdos2sys.h

nextorsys.h:
	xxd -i static/NEXTOR.SYS > nextorsys.h

ocmbiosdat.h:
	xxd -i static/OCM-BIOS.DAT > ocmbiosdat.h

execromcom.h:
	xxd -i static/execrom.com > execromcom.h

msxrom2vhd: main.c command2com.h msxdos2sys.h nextorsys.h ocmbiosdat.h execromcom.h autoexecbat.h
	cc main.c -o msxrom2vhd `pkg-config libguestfs --cflags --libs`
