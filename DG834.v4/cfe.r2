# read zignature
. cfe.z
.z/ 0x80401000 0x80421120

afr @ 0x80407ec4
afn download_mode @0x80407ec4
#afn lib_memcpy 0x80410d38

CCu "eth0" @ 0x80407f5c
CCu "Download mode ... press enter to stop" @ 0x80407f84



# stuffs from the compiled CFE
f section..text 112212 0x80401000
S 0x80401000 0x80401000 0x0001b654 0x0001b654 .text -r-x
f section..data 7912 0x8041c654
S 0x0001c654 0x8041c654 0x00001ee8 0x00001ee8 .data -rw-
f section..sbss 164 0x8041e540
S 0x0001e540 0x8041e540 0x000000a4 0x000000a4 .sbss -rw-
f section..bss 18488 0x8041e5e8
S 0x0001e544 0x8041e5e8 0x00004838 0x00004838 .bss -rw-
