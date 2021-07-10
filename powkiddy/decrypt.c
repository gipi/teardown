#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BLOCK_SIZE 0x200

uint32_t KEY[8] = { 
    0xB94B2D77,
    0x4081593c,
    0x431cb4e3,
    0xbd96f13e,
    0xa78bd002,
    0x491d0e01,
    0x70d187a4,
    0xd02a3b9a
};

uint32_t SEED[8] = {
    0x3a6d6b06,
    0x4ad3c0be,
    0x9d91b5ae,
    0x4360d2c5,
    0x825df2b3,
    0x453b38be,
    0xbd513abc,
    0xb3863703
};

/*
 * Routine used internally from the module "storage.bin" found into the
 * powkiddy-J6; it uses two prefixed arrays of eight 32bit elements, one used
 * as the key and one as a "seed".
 *
 * The algorithm works with 512 bytes block: there are sixteen rounds, each one
 * working on eight 32bit words (i.e. 4 bytes * 8 elements * 16 rounds = 512 * bytes).
 *
 * The "offset" seems completely useless since it's always used as a multiple of
 * 0x200 (in the algorithm use AND with 0x1f that is always zero).
 */
void decrypt(uint32_t* buffer, uint32_t* seed,uint32_t offset,short round) {
  int16_t index3;
  uint32_t seed7;
  uint32_t tmp;
  int16_t index;
  int16_t index2;
  uint32_t key[8];

  key[0] = KEY[0];
  key[1] = KEY[1];
  key[2] = KEY[2];
  key[3] = KEY[3];
  key[4] = KEY[4];
  key[5] = KEY[5];
  key[6] = KEY[6];
  key[7] = KEY[7];

  for (index = 0; index < 8; index = index + 1) {
    key[index] = key[index] ^ 0x5a << (offset & 0x1f) ^ seed[index];
  }

  seed7 = seed[7];
  for (index2 = 0; index2 < round; index2 = index2 + 1) {
    //index3 = (int16_t)((int)index2 << 3);
    index3 = index2 * 8;
    buffer[index3] = buffer[index3] ^ key[1];
    key[0] = key[1];
    buffer[index3 + 1] = buffer[index3 + 1] ^ key[2];
    key[1] = key[2];
    buffer[index3 + 2] = buffer[index3 + 2] ^ key[3];
    key[2] = key[3];
    tmp = key[3] ^ key[5];
    buffer[index3 + 3] = buffer[index3 + 3] ^ key[4];
    key[3] = key[4];
    buffer[index3 + 4] = buffer[index3 + 4] ^ key[5];
    key[4] = key[5];
    buffer[index3 + 5] = buffer[index3 + 5] ^ key[6];
    key[5] = key[6];
    buffer[index3 + 6] = buffer[index3 + 6] ^ key[7];
    key[6] = key[7];
    buffer[index3 + 7] = buffer[index3 + 7] ^ tmp;
    key[7] = buffer[index3 + 7] ^ seed7;
  }

  return;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <flash dump>\n", argv[0]);
        return 1;
    }

    char* path = argv[1];

    int fd = open(path, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    struct stat sb;
    if (fstat(fd, &sb) == -1)           /* To obtain file size */
        handle_error("fstat");

    fprintf(stderr, "opening '%s' with size %lu (%luMB)\n", path, sb.st_size, sb.st_size/1024/1024);

    int sector = 0x400;
    size_t offset_file = sector * BLOCK_SIZE;
    size_t size = sb.st_size - offset_file;

    fprintf(stderr, "decrypting from offset %lu for %lu bytes\n", offset_file, size);

    /* I want a page where I can read and write but the file remainis untouched */
    unsigned char* map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset_file);
    if (map == MAP_FAILED) {
        handle_error("mmap");
    }

    if (map == NULL) {
        handle_error("failed to allocate buffer");
    }

#if 1
    int flag = 0;
    size_t offset_mmap = 0;

    while (offset_file < sb.st_size) {
        decrypt(map + offset_mmap, SEED, offset_file, 0x10);
        offset_file += BLOCK_SIZE;
        offset_mmap += BLOCK_SIZE;

        fprintf(stderr, (flag++ & 1) ? "|\r" : "-\r");
    }
    fprintf(stderr, "final flag: %lu\n", flag);
    fprintf(stderr, "dumping %lu bytes\n", size);
#endif
    size_t dumped = 0;

    while (dumped < size) {
        int n = write(STDOUT_FILENO, map + dumped, size - dumped);

        if (n < 0) {
            perror("write");
            fprintf(stderr, "failed at dumped %lu\n", dumped);
            return 2;
        }
        dumped += n;
    }

    return 0;
}
