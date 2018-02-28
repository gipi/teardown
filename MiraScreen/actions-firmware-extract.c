/* actions-firmware-extract - extract firmwares like the one for Acer K330
 *
 * Copyright (C) 2014  Antonio Ospite <ao2@ao2.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <endian.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/limits.h>

/*
 * Based on cdt_parse.c from OpenEZX:
 * http://cgit.openezx.org/moto-boot-usb/tree/src/cdt_parse.c
 */

#define ACTIONS_HEADER_DISK_SIZE 64
struct actions_firmware_header {
	uint8_t magic_string[16];
	uint8_t version_string[16];
	uint8_t checksum_string[16];
	uint32_t base_address;
	uint32_t length;
	uint32_t unknown0;
	uint32_t unknown1;
};

#define ACTIONS_SECTION_DISK_SIZE 32
struct actions_firmware_section {
	uint8_t name[16];
	uint32_t start_address;
	uint32_t length;
	uint32_t unknown0;
	uint32_t unknown1;
};

static void get_string(uint8_t **srcp, uint8_t *dest, unsigned int len)
{
	memcpy(dest, *srcp, len);
	dest[len - 1] = '\0';
	*srcp += len;
}

static inline uint16_t get_le16(uint8_t **bufferp)
{
	uint16_t tmp;

	memcpy(&tmp, *bufferp, sizeof (tmp));
	*bufferp += sizeof (tmp);

	return le16toh(tmp);
}

static inline uint32_t get_le32(uint8_t **bufferp)
{
	uint32_t tmp;

	memcpy(&tmp, *bufferp, sizeof (tmp));
	*bufferp += sizeof (tmp);

	return le32toh(tmp);
}

static inline uint64_t get_le64(uint8_t **bufferp)
{
	uint64_t tmp;

	memcpy(&tmp, *bufferp, sizeof (tmp));
	*bufferp += sizeof (tmp);

	return le64toh(tmp);
}

static int parse_header(uint8_t **bufferp, struct actions_firmware_header *header)
{
	get_string(bufferp, header->magic_string, 16);

	if (strncmp((char *)header->magic_string, "ActionsFirmware", 16) != 0) {
		fprintf(stderr, "Not an Actions firmware.\n");
		return 1;
	}

	get_string(bufferp, header->version_string, 16);
	get_string(bufferp, header->checksum_string, 16);

	header->base_address = get_le32(bufferp);
	header->length = get_le32(bufferp);
	header->unknown0 = get_le32(bufferp);
	header->unknown1 = get_le32(bufferp);

	return 0;
}

static int parse_section(uint8_t **bufferp, struct actions_firmware_section *section)
{
	get_string(bufferp, section->name, 16);

	if (section->name[0] == '\0')
		return 1;

	section->start_address = get_le32(bufferp);
	section->length = get_le32(bufferp);
	section->unknown0 = get_le32(bufferp);
	section->unknown1 = get_le32(bufferp);

	return 0;
}

static void print_header(struct actions_firmware_header *header)
{
	printf("Magic: %s\n"
	       "Version: %s\n"
	       "Checksum: %s\n"
	       "Base address: %d\n"
	       "Content length: %d\n"
	       "unknown0: 0x%08x (%u)\n"
	       "unknown1: 0x%08x (%u)\n"
	       "Complete file size: %d\n",
	       header->magic_string,
	       header->version_string,
	       header->checksum_string,
	       header->base_address,
	       header->length,
	       header->unknown0, header->unknown0,
	       header->unknown1, header->unknown1,
	       header->base_address + header->length);
}

static void print_section(struct actions_firmware_section *section)
{
	printf("| %-16s | 0x%08x | 0x%08x | 0x%08x | 0x%08x |\n",
	       section->name,
	       section->start_address,
	       section->length,
	       section->unknown0,
	       section->unknown1);
}

static int dump_section(const char *path_prefix, const char *name_prefix, uint8_t *buffer, struct actions_firmware_section *section)
{
	int fd;
	int written;
	int ret = 0;
	char path[PATH_MAX] = { 0 };

	snprintf(path, PATH_MAX, "%s/%s%s", path_prefix, name_prefix, (char *)section->name);

	fd = creat(path, 0644);
	if (fd < 0) {
		perror("creat");
		ret = -1;
		goto out;
	}

	written = write(fd, buffer + section->start_address, section->length);
	if (written < 0) {
		perror("write");
		ret = -1;
		goto out_close_fd;
	}

	ret = 0;

out_close_fd:
	close(fd);
out:
	return ret;
}

static void usage(const char *name)
{
	printf("usage: %s <fimrware.bin> <dest_dir>\n", name);
}

int main(int argc, char *argv[])
{
	int fd;
	int exit_code = EXIT_SUCCESS;
	struct stat st;
	unsigned int size;
	uint8_t *buffer;
	uint8_t *buffer_iterator;
	struct actions_firmware_header header;
	struct actions_firmware_section section;
	unsigned int i = 0;
	int ret = 1;
	int err = 0;
	int dump = 1;
	const char *path_prefix;

	if (argc != 3) {
		usage(argv[0]);
		exit(1);
	}

	path_prefix = argv[2];
	err = mkdir(path_prefix, 0755);
	if (err < 0) {
		perror("mkdir");
		exit_code = EXIT_FAILURE;
		goto out;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit_code = EXIT_FAILURE;
		goto out;
	}
	if (fstat(fd, &st) < 0) {
		perror("fstat");
		exit_code = EXIT_FAILURE;
		goto out_close_fd;
	}
	size = st.st_size;

	buffer = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buffer == NULL) {
		perror("mmap");
		exit_code = EXIT_FAILURE;
		goto out_close_fd;
	}

	if (size < ACTIONS_HEADER_DISK_SIZE) {
		fprintf(stderr, "File cannot contain an Actions firmware header.\n");
		exit_code = EXIT_FAILURE;
		goto out_munmap;
	}

	/* iterate over a copy of the pointer */
	buffer_iterator = buffer;

	err = parse_header(&buffer_iterator, &header);
	if (err) {
		fprintf(stderr, "Cannot parse the header.\n");
		exit_code = EXIT_FAILURE;
		goto out_munmap;
	}

	print_header(&header);

	while((err = parse_section(&buffer_iterator, &section)) == 0) {
		print_section(&section);
		i++;

		if (strncmp((char *)section.name, "FIRM", 16) == 0) {
			uint8_t *firm_buffer_iterator = buffer + section.start_address + 0x200;
			struct actions_firmware_section firm_section;
			struct actions_firmware_section prev_section = {
				.start_address = section.start_address + 0x2000,
				.length = 0,
			};
			while((err = parse_section(&firm_buffer_iterator, &firm_section)) == 0) {

				/*
				 * unknown1 seems to be some form of checksum for
				 * firm sections, if a sections have the same
				 * checksum of the previous on they are not
				 * duplicated but refer to the same memory
				 * region, so do not increase che start
				 * address
				 */
				if (firm_section.unknown1 == prev_section.unknown1) {
					firm_section.start_address = prev_section.start_address;
				} else {
					firm_section.start_address = prev_section.start_address + prev_section.length;
				}

				printf("\t");
				print_section(&firm_section);

				if (dump)
					dump_section(path_prefix, "FIRM_", buffer, &firm_section);

				prev_section = firm_section;
			}
		} else if (strncmp((char *)section.name, "LINUX", 16) == 0) {
			continue;
		}

		if (dump)
			dump_section(path_prefix, "", buffer, &section);

	}
	printf("Found %d sections.\n", i);

	exit_code = EXIT_SUCCESS;

out_munmap:
	ret = munmap(buffer, size);
	if (ret < 0)
		perror("munmap");

out_close_fd:
	ret = close(fd);
	if (ret < 0)
		perror("close");

out:
	exit(exit_code);
}
