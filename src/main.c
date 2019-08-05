#include <guestfs.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

extern unsigned char *static_AUTOEXEC_BAT;
extern unsigned int static_AUTOEXEC_BAT_len;
extern unsigned char *static_COMMAND2_COM;
extern unsigned int static_COMMAND2_COM_len;
extern unsigned char *static_execrom_com;
extern unsigned int static_execrom_com_len;
extern unsigned char *static_MSXDOS2_SYS;
extern unsigned int static_MSXDOS2_SYS_len;
extern unsigned char *static_NEXTOR_SYS;
extern unsigned int static_NEXTOR_SYS_len;
extern unsigned char *static_OCM_BIOS_DAT;
extern unsigned int static_OCM_BIOS_DAT_len;

const size_t fs_size = 159040;

int main(int argc, char *argv[]) {

	size_t system_size = (static_OCM_BIOS_DAT_len
			      + static_MSXDOS2_SYS_len
			      + static_NEXTOR_SYS_len
			      + static_COMMAND2_COM_len
			      + static_execrom_com_len
			      + static_AUTOEXEC_BAT_len);

	if (argc != 3) {
		printf("Usage: %s <input.rom> <output.vhd>\n", argv[0]);
		exit(1);
	}

	struct stat st;
	stat(argv[1], &st);
	size_t rom_size = st.st_size;

	printf("filesystem: %ld\n", fs_size);
	printf("system: %ld\n", system_size);
	printf("user: %ld\n", rom_size);

	guestfs_h *g = guestfs_create ();
	guestfs_disk_create (g, argv[2], "raw", fs_size + system_size + rom_size, -1);
	guestfs_add_drive_opts (g, argv[2],
			        GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
				-1);
	guestfs_launch (g);
	guestfs_part_disk (g, "/dev/sda", "mbr");
	guestfs_mkfs(g, "fat", "/dev/sda1");
	guestfs_mount (g, "/dev/sda1", "/");

	/* BIOS */
	printf("Copying BIOS...\n");
	guestfs_write (g, "/OCM-BIOS.DAT",
		       static_OCM_BIOS_DAT,
		       static_OCM_BIOS_DAT_len);

	/* OS */
	printf("Copying OS...\n");
	guestfs_write (g, "/MSXDOS2.SYS",
		       static_MSXDOS2_SYS,
		       static_MSXDOS2_SYS_len);

	guestfs_write (g, "/NEXTOR.SYS",
		       static_NEXTOR_SYS,
		       static_NEXTOR_SYS_len);

	guestfs_write (g, "/COMMAND2.COM",
		       static_COMMAND2_COM,
		       static_COMMAND2_COM_len);

	/* ROM LOADER */
	printf("Copying loader...\n");
	guestfs_write (g, "/EXECROM.COM",
		       static_execrom_com,
		       static_execrom_com_len);

	guestfs_write (g, "/AUTOEXEC.BAT",
		       static_AUTOEXEC_BAT,
		       static_AUTOEXEC_BAT_len);

	/* ROM */
	printf("Copying rom...\n");
	guestfs_upload (g, argv[1], "/USER.ROM");

	guestfs_umount (g, "/");
	guestfs_shutdown (g);
	guestfs_close (g);
}
