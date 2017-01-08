# read zignature
. cfe.z
.z/ 0x80401000 0x80421120

afr @ 0x80407ec4
afn download_mode @0x80407ec4
afn first @0x80407a80
afn sercomm_check @0x80407afc
afn cfe_open @ 0x80411efc
afn maybe_leds @ 0x80407ac4
afn cfe_read @ 0x80411ff8
afn cfe_readblk @ 0x80411f98
#afn lib_memcpy 0x80410d38

CCu "eth0" @ 0x80407f5c
CCu "Download mode ... press enter to stop" @ 0x80407f84
CCu "sErCoMm" @ 0x80407b50
CCu "sErCoMm\x01" @ 0x80407f10
CCu base64:YWRkcmVzcyBsaWtlIDB4ZmZmZTA0MDAgYXJlIEdQSU8gYWRkcmVzc2VzIChtYXliZSBsZWRzPyk= @ 0x80407a80
CCu base64:aGVyZSBzaW1wbHkgY2hlY2sgaWYgY2hhcnMgYXJlIGF2YWlsYWJsZSAocmV0dXJuIHZhbHVlID09IDAgPT4gbm90aGluZyB0byBwcm9jZXNzKQ== @ 0x804084b8
CCu base64:djEgPSAoc3RydWN0IGV0aGhkcikuaF9wcm90bw== @ 0x80407fcc
CCu base64:RVRIX1BfTkZUUA== @ 0x80407fd0
CCu base64:aWYgKGhfcHJvdG8gIT0gRVRIX1BfTkZUUCk= @ 0x80407fd4



# stuffs from the compiled CFE
f section..text 112212 0x80401000
S 0x80401000 0x80401000 0x0001b654 0x0001b654 .text -r-x
f section..data 7912 0x8041c654
S 0x0001c654 0x8041c654 0x00001ee8 0x00001ee8 .data -rw-
f section..sbss 164 0x8041e540
S 0x0001e540 0x8041e540 0x000000a4 0x000000a4 .sbss -rw-
f section..bss 18488 0x8041e5e8
S 0x0001e544 0x8041e5e8 0x00004838 0x00004838 .bss -rw-
