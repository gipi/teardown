/*
 * Implements the checksum as seen in the update application.
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "actions.h"


/*
 * The sections are aligned to 0x20 bytes and the names are
 * at the start of them.
 */
int findSection(void* buffer, const char* name) {
    unsigned int cycle;
    void *ptr2buffer = buffer;
    for (cycle = 0 ; cycle < 0x20 ; cycle++, ptr2buffer += 0x20) {
        int result = strcmp(ptr2buffer, name);

        if (!result) {
            return cycle * 0x20;
        }
    }

    return -1;
}

// FIXME: it doesn't handle not 32-bit aligned buffer
uint32_t checksum(void* buffer, uint32_t size) {
    uint32_t returnValue = 0; /* this is the final checksum */
    uint32_t* ptr2buffer = (uint32_t*)buffer;

    for (; ptr2buffer < buffer + size ; ptr2buffer++) {
        returnValue += *ptr2buffer;
    }

    return returnValue;
}

uint32_t checksum_file(int fileDescriptor, uint32_t startOffset, uint32_t size) {
    /* first of all we seek to the specified offset */
    lseek(fileDescriptor, startOffset, SEEK_SET);

    void* fileContents = malloc(size);

    int count = read(fileDescriptor, fileContents, size);
    if (count < size) {
        fprintf(stderr, "fatal: returned less bytes than expected (%08x instead of %08x)\n", count, size);
        return -1;
    }

    uint32_t ret = checksum(fileContents, size);

    free(fileContents);

    return ret;
}


void handle_CHECKSUM(int fd, void* header) {
    int checksumOffset = findSection(header, "CHECKSUM");

    fprintf(stdout, "CHECKSUM section at offset 0x%08x\n", checksumOffset);

    struct ActionsChecksumSection* checksumSection = (struct ActionsChecksumSection*)((void*)header + checksumOffset);

    fprintf(stdout, " offset:\t0x%08x\n", checksumSection->start);
    fprintf(stdout, " length:\t0x%08x\n", checksumSection->length);
    fprintf(stdout, " is encrypted:\t0x%08x\n", checksumSection->isEncrypted);
    fprintf(stdout, " checksum:\t0x%08x\n", checksumSection->checksum);

    uint32_t calculatedChecksum = checksum_file(fd, checksumSection->start, checksumSection->length);

    fprintf(stdout, " checksum\t0x%08x\n", calculatedChecksum);
}

