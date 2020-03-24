#include <stdint.h>
#include "actions.h"
#include "z.h"

void FIRM_dump(int fd, void* header) {
    int firmOffset = findSection(header, "FIRM");
    fprintf(stdout, "FIRM section at offset 0x%08x\n", firmOffset);

    struct ActionsFIRMSection* firmSection = (struct ActionsFIRMSection*)((void*)header + firmOffset);

    fprintf(stdout, " offset:\t0x%08x\n", firmSection->start);
    fprintf(stdout, " length:\t0x%08x\n", firmSection->final_size);

    FILE *file = fdopen(fd, "r");

    fseek(file, firmSection->start, SEEK_SET);

    int ret = inf(file, stdout);

    if (ret != Z_OK) {
        zerr(ret);
    }
}

