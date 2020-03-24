#ifndef __ACTIONS_H__
#define __ACTIONS_H__
#include <stdint.h>
#define ACTIONS_HEADER_SIZE 0x400

struct ActionsHeader {
    char magic[16];
    char version[16];
    char checksum[16];
    uint32_t base_address;
    uint32_t length;
    uint32_t unknown0;
    uint32_t unknown1;
};

/* these below are header and not the sections themselves */
struct ActionsChecksumSection {
    char name[16];
    uint32_t start;
    uint32_t length;
    uint32_t isEncrypted;
    uint32_t checksum;
};

struct ActionsFIRMSection {
    char name[16];
    uint32_t start;
    uint32_t unk1;
    uint32_t final_size;
    uint32_t unk2;
};

struct ActionsSection {
    char name[16];
    uint32_t offset;
    uint32_t length;
    uint32_t unk0;
    uint32_t unk1;
};

struct ActionsSections {
    unsigned int count;
    struct ActionsSection** sections;
};

int findSection(void* buffer, const char* name);
#endif
