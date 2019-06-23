#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "actions.h"

void usage(const char* progname) {
    fprintf(stderr, "usage: %s <firmware>\n", progname);
    exit(1);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage(argv[0]);
    }

    const char* firmwarePath = strdup(argv[1]);
    if (!firmwarePath) {
        perror("failed to allocate path");
        exit(1);
    }

    int fd = open(firmwarePath, O_RDONLY);

    if (fd < 0) {
        perror("I could not open the file");
        exit(1);
    }

    char header[ACTIONS_HEADER_SIZE];

    int count = read(fd, header, ACTIONS_HEADER_SIZE);
    if (count < ACTIONS_HEADER_SIZE) {
        fprintf(stderr, "fatal: I could not read the header\n");
        exit(1);
    }

    decompress_FIRM(fd, header);

    return 0;
}
