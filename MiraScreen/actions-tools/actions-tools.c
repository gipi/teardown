#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "actions.h"

void usage(const char* progname) {
    fprintf(stderr, "usage: %s <firmware>\n", progname);
    fprintf(stderr, "\nUtility for unpacking firmware of AM7XXX devices.\n");
    fprintf(stderr, "It extracts the various sections of the firmware in the current directory.\n");
    exit(1);
}


struct ActionsHeader* parseHeader(int fd) {
    struct ActionsHeader* header = malloc(sizeof(struct ActionsHeader));
    int n = read(fd, header, sizeof(struct ActionsHeader));

    if (n < sizeof(struct ActionsHeader)) {
        if (n < 0) {
            perror("read() header");
        }
        fprintf(stderr, "fatal: not enough data for the header");
        free(header);

        return NULL;
    }

    fprintf(stdout, "ActionsHeader\n");
    fprintf(stdout, " magic: %s\n", header->magic);
    fprintf(stdout, " version: %s\n", header->version);
    fprintf(stdout, " offset: 0x%x\n", header->base_address);
    fprintf(stdout, " length: 0x%x\n", header->length);

    return header;
}


void sectionInfo(struct ActionsSection section) {
    fprintf(stdout, "section's name: %s\n", section.name);
    fprintf(stdout, " start offset: 0x%x\n", section.offset);
    fprintf(stdout, " length: 0x%x\n", section.length);
    fprintf(stdout, " unk0: 0x%x\n", section.unk0);
    fprintf(stdout, " unk1: 0x%x\n", section.unk1);
}


void sectionDump(int fd_input, struct ActionsSection section) {
    fprintf(stdout, "dumping section '%s'\n", section.name);
    int fd_output = creat(section.name, S_IRUSR | S_IWUSR);
    if (fd_output < 0) {
        perror("open() output file");
    }

    char* buffer = malloc(section.length);
    if (!buffer) {
        fprintf(stderr, "not enough memory for section '%s' with size %d\n", section.name, section.length);
        goto _exit;
    }

    /* 
     * ATTENTION: bc this lseek() you cannot call this function when you are
     * reading a continuous stream from the input file descriptor.
     */
    off_t result = lseek(fd_input, section.offset, SEEK_SET);
    if (result < 0) {
        perror("lseek()");
        goto _exit_and_free;
    }

    int n = read(fd_input, buffer, section.length);
    if (n < 0) {
        perror("read()");
        goto _exit_and_free;
    }

    n = write(fd_output, buffer, section.length);
    if (n < 0) {
        perror("write()");
        goto _exit_and_free;
    }

_exit_and_free:
    free(buffer);
_exit:
    close(fd_output);

    return;
}


struct ActionsSections* parseSections(int fd, struct ActionsHeader header, int dump) {
    fprintf(stdout, "Actions sections\n");
    off_t result = lseek(fd, header.base_address, SEEK_SET);
    if (result < 0) {
        perror("lseek()");
        return NULL;
    }

    size_t size_section = sizeof(struct ActionsSection);

    size_t count = 0;
    struct ActionsSection** list_sections  = malloc(sizeof(struct ActionsSection*) * 20); // FIXME: realloc when necessary
    struct ActionsSections* sections = malloc(sizeof(struct ActionsSections)); // FIXME: realloc when necessary

    for (;;) {
        struct ActionsSection* section = malloc(size_section);
        int n = read(fd, section, size_section);

        if (n < 0) {
            perror("read()");
            free(section);
            break;
        }

        /* at the first NULL entry we exit */
        if (!section->name[0]) {
            free(section);
            break;
        }

        sectionInfo(*section);

        list_sections[count++] = section;
    }

    sections->count = count;
    sections->sections = list_sections;

    return sections;
}


int isSectionDumpable(struct ActionsSection section) {
    return strcmp(section.name, "LINUX");
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

    struct ActionsHeader* header = parseHeader(fd);
    if (!header) {
        exit(2);
    }

    struct ActionsSections* sections = parseSections(fd, *header, 1);
    if (!sections) {
        exit(3);
    }

    unsigned int count;
    for (count = 0 ; count < sections->count ; count++) {
        struct ActionsSection section = *sections->sections[count];
        if (!isSectionDumpable(section))
            continue;
        sectionDump(fd, section);
    }

    // FIRM_dump(fd, header);
    //
    free((void*)firmwarePath);
    free(sections->sections);
    // TODO: free all the items in sections->sections
    free(sections);

    return 0;
}
