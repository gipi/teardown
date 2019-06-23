#ifndef __ACTIONS_H__
#define __ACTIONS_H__
#define ACTIONS_HEADER_SIZE 0x400


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
    uint32_t size;
    uint32_t unk2;
};

int findSection(void* buffer, const char* name);
#endif
