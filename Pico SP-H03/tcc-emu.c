/*
 * Loads and executes code wannabe running on a TCC8900 chips allocating
 * memory to emulate peripherals' memory.
 *
 * You can use this with Qemu.
 *
 *  $ export QEMU_LD_PREFIX=/usr/arm-linux-gnueabi
 *  $ qemu-arm -cpu arm1176 -g 4444 tcc-emu tccvirus.bin
 *
 * and from another terminal
 *
 *  $ gdb-multiarch tccvirus
 *
 * Magically all the symbols will be visible from GDB!
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


#define GPIO_MEMORY_ADDR 0xf0100000
#define GPIO_SIZE 0x400000

#define FIRMWARE_MEMORY_ADDR 0x00001000


void usage(const char progname[]) {
    fprintf(stderr, "usage: %s <firmware.bin>\n", progname);
    exit(1);
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        usage(argv[0]);
    }

    char* filepath = argv[1];

    struct stat file_info;
    int retVal = stat(filepath, &file_info);

    if (retVal == -1) {
        perror("I could not stat file");
        goto fatal;
    }

    void * gpio = mmap((void*)GPIO_MEMORY_ADDR, GPIO_SIZE, PROT_READ | PROT_WRITE , MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (gpio == MAP_FAILED) {
        perror("allocate gpio failed");
        goto fatal;
    }

    int fw_fd = open(filepath, O_RDONLY);
    if (fw_fd == -1) {
        perror("I could not open file");
        goto fatal;
    }

    void* (*firmware)();

    firmware = mmap((void*)FIRMWARE_MEMORY_ADDR, file_info.st_size, PROT_READ | PROT_EXEC , MAP_FIXED | MAP_PRIVATE, fw_fd, 0);

    if (gpio == MAP_FAILED) {
        perror("allocate firmware failed");
        goto fatal;
    }

    fprintf(stderr, "-- Executing firmware at address 0x%08x--\n", FIRMWARE_MEMORY_ADDR);
    firmware();
fatal:

    return 0;
}
