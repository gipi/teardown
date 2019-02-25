// https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html
#include <stdint.h>

struct TWDCFG{
    uint32_t en:1;
    uint32_t zero:2;
    uint32_t ien:1;
    uint32_t tclksel:3;
    uint32_t unused:10;
};

int main() {
    uint32_t TWDCFG_ADDR = 0xf0403070;

    struct TWDCFG* reg = (struct TWDCFG*)TWDCFG_ADDR;

    reg->en = 1;
    reg->tclksel = 0x04;
}
