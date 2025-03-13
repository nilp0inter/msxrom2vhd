#include <guestfs.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <config.h>

extern unsigned char static_AUTOEXEC_BAT[];
extern unsigned int static_AUTOEXEC_BAT_len;
extern unsigned char static_COMMAND2_COM[];
extern unsigned int static_COMMAND2_COM_len;
extern unsigned char static_execrom_com[];
extern unsigned int static_execrom_com_len;
extern unsigned char static_MSXDOS2_SYS[];
extern unsigned int static_MSXDOS2_SYS_len;
extern unsigned char static_NEXTOR_SYS[];
extern unsigned int static_NEXTOR_SYS_len;
extern unsigned char static_OCM_BIOS_DAT[];
extern unsigned int static_OCM_BIOS_DAT_len;

const size_t fs_size = 156621;

// Given a ROM file, it creates a .VHD file with the ROM and the necessary
// files to boot into it.
int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);

	size_t system_size = (static_OCM_BIOS_DAT_len
			      + static_MSXDOS2_SYS_len
			      + static_NEXTOR_SYS_len
			      + static_COMMAND2_COM_len
			      + static_execrom_com_len
			      + static_AUTOEXEC_BAT_len);

	if (argc != 3) {
		printf("%s ( %s )\n", PACKAGE_STRING, PACKAGE_URL);
		printf("Usage: %s <input.rom> <output.vhd>\n", argv[0]);
		exit(1);
	}

	struct stat st;
	stat(argv[1], &st);
	size_t rom_size = st.st_size;

	guestfs_h *g = guestfs_create();
	if (g == NULL) {
		perror("failed to create libguestfs handle");
		exit(EXIT_FAILURE);
	}

	printf("Creating .VHD file (fs: %ld, sys: %ld, user: %ld)... ", fs_size, system_size, rom_size);
	if (guestfs_disk_create(g, argv[2], "raw", fs_size + system_size + rom_size, -1) == -1)
		exit(EXIT_FAILURE);
	if (guestfs_add_drive_opts(g, argv[2], GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw", -1) == -1)
		exit(EXIT_FAILURE);
	printf("OK\n");

	printf("Partitioning... ");
	if (guestfs_launch(g) == -1)
		exit(EXIT_FAILURE);
	if (guestfs_part_disk(g, "/dev/sda", "mbr") == -1)
		exit(EXIT_FAILURE);
	printf("OK\n");

	printf("Formatting... ");
	if (guestfs_mkfs(g, "fat", "/dev/sda1") == -1)
		exit(EXIT_FAILURE);
	printf("OK\n");

	printf("Mounting... ");
	if (guestfs_mount(g, "/dev/sda1", "/") == -1)
		exit(EXIT_FAILURE);
	printf("OK\n");

	printf("Copying files... ");
	/* BIOS */
	printf("BIOS");
	if (guestfs_write(g, "/OCM-BIOS.DAT", static_OCM_BIOS_DAT, static_OCM_BIOS_DAT_len) == -1)
		exit(EXIT_FAILURE);

	/* OS */
	printf(" OS");
	if (guestfs_write(g, "/MSXDOS2.SYS", static_MSXDOS2_SYS, static_MSXDOS2_SYS_len) == -1)
		exit(EXIT_FAILURE);

	if (guestfs_write(g, "/NEXTOR.SYS", static_NEXTOR_SYS, static_NEXTOR_SYS_len) == -1)
		exit(EXIT_FAILURE);

	if (guestfs_write(g, "/COMMAND2.COM", static_COMMAND2_COM, static_COMMAND2_COM_len) == -1)
		exit(EXIT_FAILURE);

	/* ROM LOADER */
	printf(" LOADER");
	if (guestfs_write(g, "/EXECROM.COM", static_execrom_com, static_execrom_com_len) == -1)
		exit(EXIT_FAILURE);

	if (guestfs_write(g, "/AUTOEXEC.BAT", static_AUTOEXEC_BAT, static_AUTOEXEC_BAT_len) == -1)
		exit(EXIT_FAILURE);

	/* ROM */
	printf(" ROM");
	if (guestfs_upload(g, argv[1], "/USER.ROM") == -1)
		exit(EXIT_FAILURE);
	printf("\n");

	printf("Unmounting... ");
	if (guestfs_umount(g, "/") == -1)
		exit(EXIT_FAILURE);
	printf("OK\n");

	if (guestfs_shutdown(g) == -1)
		exit(EXIT_FAILURE);

	guestfs_close(g);
	exit(EXIT_SUCCESS);
}
