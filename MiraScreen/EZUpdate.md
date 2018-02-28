# EZUpdate

Below some attempts to reverse the update application (I don't
really know if the EZCast update process works for this device
but meanwhile...).


```
[0x004dc2a4]> iz~http
vaddr=0x0055e0ec paddr=0x0015e0ec ordinal=2505 sz=41 len=40 section=.data type=ascii string=https://www.iezvu.com/upgrade/ota_rx.php
vaddr=0x0055e274 paddr=0x0015e274 ordinal=2519 sz=45 len=44 section=.data type=ascii string=http://www.iezvu.com/indexhelp.php?tt=1&l=en
vaddr=0x0055e2a4 paddr=0x0015e2a4 ordinal=2520 sz=43 len=42 section=.data type=ascii string=http://www.iezvu.com/repair_tool.php?&l=en
[0x004dc2a4]> /c 55e0ec
0x0040932b   # 5: push str.https:__www.iezvu.com_upgrade_ota_rx.php
[0x004dc2a4]> pdf @ 0x0040932b
/ (fcn) sub.MSVCRT.dll_free_320 102
|   sub.MSVCRT.dll_free_320 ();
|              ; CALL XREF from 0x004093a0 (hit0_0 + 117)
|           0x00409320      56             push esi
|           0x00409321      8bf1           mov esi, ecx
|           0x00409323      57             push edi
|           0x00409324      8d8660030000   lea eax, [esi + 0x360]
|           0x0040932a      50             push eax
|           ;-- hit0_0:
|           0x0040932b      68ece05500     push str.https:__www.iezvu.com_upgrade_ota_rx.php ; 0x55e0ec ; "https://www.iezvu.com/upgrade/ota_rx.php"
|           0x00409330      e82be70100     call fcn.00427a60
|           0x00409335      8bf8           mov edi, eax
|           0x00409337      83c408         add esp, 8
|           0x0040933a      85ff           test edi, edi
|       ,=< 0x0040933c      7503           jne 0x409341
|       |   0x0040933e      5f             pop edi
|       |   0x0040933f      5e             pop esi
|       |   0x00409340      c3             ret
|       `-> 0x00409341      53             push ebx
|           0x00409342      8d8ee0020000   lea ecx, [esi + 0x2e0]
|           0x00409348      8d9e60020000   lea ebx, [esi + str..rdata] ; 0x260 ; ".rdata"
|           0x0040934e      51             push ecx
|           0x0040934f      53             push ebx
|           0x00409350      57             push edi                    ; size_t size
|           0x00409351      e80afb0000     call fcn.00418e60
|           0x00409356      57             push edi
|           0x00409357      ff1568f64d00   call dword [sym.imp.MSVCRT.dll_free] ; 0x4df668 ; "^\xdc\x10"
|           0x0040935d      8d9660010000   lea edx, [esi + 0x160]
|           0x00409363      52             push edx
|           0x00409364      53             push ebx
|           0x00409365      e8a6e10100     call sub.USER32.dll_SendMessageA_510 ; lresult SendMessageA(HWND hWnd, WindowsMessage Msg, WPARAM wParam, LPARAM lParam)
|           0x0040936a      83c418         add esp, 0x18
|           0x0040936d      85c0           test eax, eax
|           0x0040936f      5b             pop ebx
|       ,=< 0x00409370      7405           je 0x409377
|       |   0x00409372      5f             pop edi
|       |   0x00409373      33c0           xor eax, eax
|       |   0x00409375      5e             pop esi
|       |   0x00409376      c3             ret
|       `-> 0x00409377      8bce           mov ecx, esi
|           0x00409379      e872000000     call sub.MSVCRT.dll_malloc_3f0 ;  void *malloc(size_t size)
|           0x0040937e      5f             pop edi
|           0x0040937f      b801000000     mov eax, 1
|           0x00409384      5e             pop esi
\           0x00409385      c3             ret
[0x004dc2a4]> pdf @ fcn.00427a60
/ (fcn) fcn.00427a60 39
|   fcn.00427a60 ();
|              ; CALL XREF from 0x00409330 (sub.MSVCRT.dll_free_320)
|           0x00427a60      51             push ecx
|           0x00427a61      8b4c240c       mov ecx, dword [esp + 0xc]  ; [0xc:4]=0xffff
|           0x00427a65      8b542408       mov edx, dword [esp + 8]    ; [0x8:4]=4
|           0x00427a69      8d442400       lea eax, [esp]
|           0x00427a6d      6a00           push 0
|           0x00427a6f      50             push eax
|           0x00427a70      51             push ecx
|           0x00427a71      52             push edx                    ; size_t size
|           0x00427a72      c74424100000.  mov dword [esp + 0x10], 0
|           0x00427a7a      e8b1e9feff     call sub.MSVCRT.dll_malloc_430 ;  void *malloc(size_t size)
|           0x00427a7f      8b442410       mov eax, dword [esp + 0x10] ; [0x10:4]=184
|           0x00427a83      83c414         add esp, 0x14
\           0x00427a86      c3
```

``fcn.00427a60()`` looks like a ``malloc()`` from the name, but in reality
it's a ``curl_*`` related function


```
                            .--------------------------------------------------------------.
                            | [0x416430] ;[gd]                                             |
                            | (fcn) sub.MSVCRT.dll_malloc_430 447                          |
                            |   sub.MSVCRT.dll_malloc_430 ();                              |
                            |    ; CALL XREF from 0x00427a7a (fcn.00427a60)                |
                            |    ; CALL XREF from 0x00415d25 (sub.MSVCRT.dll_sprintf_c30)  |
                            | sub esp, 0xc                                                 |
                            | push esi                                                     |
                            | push edi                                                     |
                            | or edi, 0xffffffff                                           |
                            | push 1                                                       |
                            | mov dword [esp + 0xc], edi                                   |
                            |    ; 0x4df66c                                                |
                            |    ; "f\xdc\x10"                                             |
                            | call dword [sym.imp.MSVCRT.dll_malloc];[ga]                  |
                            | mov dword [esp + 0x10], eax                                  |
                            | add esp, 4                                                   |
                            | mov byte [eax], 0                                            |
                            | mov dword [esp + 0x10], 0                                    |
                            | call sym.ActionsMPUpdate.exe_curl_easy_init;[gb]             |
                            | mov esi, eax                                                 |
                            | test esi, esi                                                |
                            | je 0x4165cb;[gc]                                             |
                            `--------------------------------------------------------------'
                                    f t
        .---------------------------' '-------------------------------.
        |                                                             |
        |                                                             |
.---------------------------------------------------------.     .-------------------------------------------------------.
|  0x416465 ;[gi]                                         |     |  0x4165cb ;[gc]                                       |
|      ; [0x18:4]=64                                      |     |      ; [0x4df6c0:4]=0x10dd32 reloc.MSVCRT.dll__iob_50 |
| mov eax, dword [esp + 0x18]                             |     |      ; "2\xdd\x10"                                    |
| push ebp                                                |     | mov eax, dword sym.imp.MSVCRT.dll__iob                |
| push eax                                                |     |    ; 0x55f640                                         |
| push 0x2712  10002 CURLOPT_URL                          |     |    ; "curl_easy_init fail\n"                          |
| push esi                                                |     | push str.curl_easy_init_fail_n                        |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |     |    ; '@'                                              |
|    ; [0x2c:4]=0                                         |     | add eax, 0x40                                         |
|    ; ','                                                |     |    ; "%s\n"                                           |
| mov edi, dword [esp + 0x2c]                             |     | push 0x55f6a4                                         |
| add esp, 0xc                                            |     | push eax                                              |
| push edi                                                |     |    ; 0x4df69c                                         |
| push 0x271f  10015            CURLOPT_POSTFIELDS        |     | call dword [sym.imp.MSVCRT.dll_fprintf];[gu]          |
| push esi                                                |     | add esp, 0xc                                          |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |     | mov eax, edi                                          |
| or ecx, 0xffffffff                                      |     | pop edi                                               |
| xor eax, eax                                            |     | pop esi                                               |
| add esp, 0xc                                            |     | add esp, 0xc                                          |
| repne scasb al, byte es:[edi]                           |     | ret                                                   |
| not ecx                                                 |     `-------------------------------------------------------'
| dec ecx                                                 |
| push ecx                                                |
|    ; '<'                                                |
|    ; '<'                                                |
| push 0x3c 60 POSTFIELDSIZE                              |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
| add esp, 0xc                                            |
| push 0                                                  |
|    ; '@'                                                |
|    ; '@'                                                |
| push 0x40 64 SSL_VERIFYPEER                             |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
| add esp, 0xc                                            |
| push 0                                                  |
|    ; 'Q'                                                |
|    ; 'Q'                                                |
| push 0x51 81 SSL_VERIFYHOST                             |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
|    ; 0x55f6a8                                           |
|    ; "Content-Type: application/json; charset=utf-8"    |
| push str.Content_Type:_application_json__charset_utf_8  |
| push 0                                                  |
| call sym.ActionsMPUpdate.exe_curl_slist_append;[gf]     |
| add esp, 0x14                                           |
| mov ebp, eax                                            |
| push ebp                                                |
| push 0x2727  10023 CURLOPT_HTTPHEADER                   |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
| add esp, 0xc                                            |
| push 0x4165f0                                           |
| push 0x4e2b  20011 CURLOPT_WRITEFUNCTION                |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
| add esp, 0xc                                            |
| lea ecx, [esp + 0x10]                                   |
| push ecx                                                |
| push 0x2711 10011 CURLOPT_WRITEDATA                     |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_setopt;[ge]      |
| push esi                                                |
| call sym.ActionsMPUpdate.exe_curl_easy_perform;[gg]     |
| add esp, 0x10                                           |
| test eax, eax                                           |
| jne 0x41658f;[gh]                                       |
`---------------------------------------------------------'
        f t
--------' '----------------------------.
```


```
[0x004dc2a4]> f~update
0x004fbab0 29 str.nextupdate_before_thisupdate
0x004fbb00 26 str.error_in_thisupdate_field
0x004fbb1c 26 str.error_in_nextupdate_field
0x00501fc4 19 str.BN_BLINDING_update
0x0054c184 98 str._s:_update_zone_table___change_the_original_sublk__u_tosublk__u_for_logical_block__u_of_zone__u._n
0x005616cc 15 str.update_succeed
0x005616dc 13 str.update_error
```

## MACRO

``curl`` uses a preprocessor macro to create the ``CURLOPT_*`` variable

```
#define CINIT(na,t,nu) CURLOPT_ ## na = CURLOPTTYPE_ ## t + nu
```

the easiest way is to use directly the preprocessor

```
$ gcc -E /usr/include/x86_64-linux-gnu/curl/curl.h | grep CURLOPT
```

```
vaddr=0x0055f4bc paddr=0x0015f4bc ordinal=2749 sz=40 len=39 section=.data type=ascii string={\n\t"version":\t1,\n\t"vendor":\t"Actions"\n}
:> /c 55f4bc
0x00415375   # 5: push str.__n_t_version_:_t1__n_t_vendor_:_t_Actions__n_

```

```
:> ps @ str.__n_________version_:______1__n_________vendor_:________ezcast___n_________mac_address_:___authorshxj___n_________softap_ssid_:___000000_00000000___n_________firmware_version_:______0__n__n 
{
        "version":      1,
        "vendor":       "ezcast",
        "mac_address":  "authorshxj",
        "softap_ssid":  "000000-00000000",
        "firmware_version":     "0"
}
```

```
:> /c 55e184
0x00409e73   # 5: push str.Upgrade_Success__Please_Restart_EZCast_Dongle_
```

``fcn.004093d0()`` starts a thread that calls

```
    .-----------------------------------------------------------------------.
    | [0x4093d0] ;[gb]                                                      |
    | (fcn) fcn.004093d0 20                                                 |
    |   fcn.004093d0 ();                                                    |
    |    ; CALL XREF from 0x00409312 (fcn.004092f0 + 34)                    |
    |    ; CALL XREF from 0x0040a09e (loc.00409f0d + 401)                   |
    | 0x004093d0 6a00           push 0                                      |
    | 0x004093d2 6a00           push 0                                      |
    | 0x004093d4 6a00           push 0                                      |
    | 0x004093d6 6a00           push 0                                      |
    | 0x004093d8 51             push ecx                                    |
    | | ;-- hit0_0:                                                         |
    |    ; 0x409390                                                         |
    |    ; "V\x8bt$\bj\x05\x8b\xce\xe8\x82\x06"                             |
    | 0x004093d9 6890934000     push fcn.00409390                           |
    | 0x004093de e8bf280d00     call sub.MFC42.DLL_AfxBeginThread_ca2;[ga]  |
    | 0x004093e3 c3             ret                                         |
    `-----------------------------------------------------------------------'

                                         .-------------------------------------------------------------------.
                                         | [0x409390] ;[gd]                                                  |
                                         | (fcn) fcn.00409390 57                                             |
                                         |   fcn.00409390 ();                                                |
                                         | ; var int local_8h @ esp+0x8                                      |
                                         |    ; DATA XREF from 0x004093d9 (hit0_0 + 0)                       |
                                         | 0x00409390 56             push esi                                |
                                         |    ; [0x8:4]=4                                                    |
                                         | 0x00409391 8b742408       mov esi, dword [local_8h]               |
                                         | 0x00409395 6a05           push 5                                  |
                                         | 0x00409397 8bce           mov ecx, esi                            |
                                         | 0x00409399 e882060000     call sub.USER32.dll_KillTimer_a20;[ga]  |
                                         | 0x0040939e 8bce           mov ecx, esi                            |
                                         | 0x004093a0 e87bffffff     call update;[gb]                        |
                                         | 0x004093a5 85c0           test eax, eax                           |
                                         | 0x004093a7 7510           jne 0x4093b9;[gc]                       |
                                         `-------------------------------------------------------------------'
                                                 f t
            .------------------------------------' '--------------------------------.
            |                                                                       |
            |                                                                       |
    .-------------------------------------------------------------------.     .-------------------------------------------------------------------.
    |  0x4093a9 ;[ge]                                                   |     |  0x4093b9 ;[gc]                                                   |
    | 0x004093a9 6a03           push 3                                  |     |      ; JMP XREF from 0x004093a7 (fcn.00409390)                    |
    | 0x004093ab 8bce           mov ecx, esi                            |     | 0x004093b9 6a06           push 6                                  |
    | 0x004093ad e86e060000     call sub.USER32.dll_KillTimer_a20;[ga]  |     | 0x004093bb 8bce           mov ecx, esi                            |
    | 0x004093b2 b801000000     mov eax, 1                              |     | 0x004093bd e85e060000     call sub.USER32.dll_KillTimer_a20;[ga]  |
    | 0x004093b7 5e             pop esi                                 |     | 0x004093c2 b801000000     mov eax, 1                              |
    | 0x004093b8 c3             ret                                     |     | 0x004093c7 5e             pop esi                                 |
    `-------------------------------------------------------------------'     | 0x004093c8 c3             ret                                     |
                                                                              `-------------------------------------------------------------------'
```

```
$ curl -X POST -H "Content-type: application/json; charset=utf-8" -i https://www.iezvu.com/upgrade/ota_rx.php -d'{
        "version":      1,
        "vendor":       "ezcast",
        "mac_address":  "authorshxj",
        "softap_ssid":  "000000-00000000",
        "firmware_version":     "0"
}'
HTTP/2 200
server: nginx
date: Wed, 16 Aug 2017 16:03:02 GMT
content-type: text/html; charset=utf-8
vary: Accept-Encoding
x-powered-by: PHP/5.5.9-1ubuntu4.21

{"ota_conf_file":"http://cdn.iezvu.com/upgrade/ezcast/ezcast-16224000.conf","ota_fw_file":"http://cdn.iezvu.com/upgrade/ezcast/ezcast-16224000.gz","ota_enforce":true}
$ curl -X POST -H "Content-type: application/json; charset=utf-8" -i https://www.iezvu.com/upgrade/ota_rx.php -d'{
        "version":      1,
        "vendor":       "mirawire_8252n",
        "mac_address":  "authorshxj",
        "softap_ssid":  "000000-00000000",
        "firmware_version":     "0"
}'
HTTP/2 200
server: nginx
date: Wed, 16 Aug 2017 16:14:38 GMT
content-type: text/html; charset=utf-8
vary: Accept-Encoding
x-powered-by: PHP/5.5.9-1ubuntu4.21

{"ota_conf_file":"http://cdn.iezvu.com/upgrade/mirawire_8252n_8M/mirawire_8252n_8M-16285000.conf","ota_fw_file":"http://cdn.iezvu.com/upgrade/mirawire_8252n_8M/mirawire_8252n_8M-16285000.gz","ota_enforce":false}
```

## USB

Another aspect of this app that is interesting is the ``USB`` protocol, at ``0x00402e90``
seems to start the USB protocol.

```
[0x004dc2a4]> /c 55fb50
0x00418882   # 5: push str.USB_Mass_Storage_Device
0x0055fb4d   # 3: jb str.USB_Mass_Storage_Device
0x0055fb4e   # 2: jb str.USB_Mass_Storage_Device
```

Lurking the code we can see that here seems to start the ``USB`` opening

```
.----------------------------------------------------------------.
|  0x40ba4b ;[gg]                                               ||
|      ; JMP XREF from 0x0040baec (sub.USER32.dll_wsprintfA_a10)||
| push edi                                                      ||
|    ; "@"                                                      ||
| lea eax, [esp + 0x18]                                         ||
|    ; 0x55e414                                                 ||
|    ; "\\\\.\\HCD%d"                                           ||
| push str.__._HCD_d                                            ||
| push eax                                                      ||
|    ; 0x4df734                                                 ||
| call dword [sym.imp.USER32.dll_wsprintfA];[gd]                ||
| add esp, 0xc                                                  ||
| lea ecx, [esp + 0x14]                                         ||
| push 0                                                        ||
| push 0                                                        ||
| push 3                                                        ||
| push 0                                                        ||
| push 2                                                        ||
| push 0x40000000                                               ||
| push ecx                                                      ||
|    ; 0x4df0d8                                                 ||
|    ; "H\xe0\x10"                                              ||
| call dword [sym.imp.KERNEL32.dll_CreateFileA];[ge]            ||
| mov esi, eax                                                  ||
| cmp esi, 0xffffffffffffffff                                   ||
| je 0x40bae4;[gf]                                              ||
`----------------------------------------------------------------'
```

``\\.\HD0`` seems the [way to access USB](http://www.cplusplus.com/forum/beginner/14634/) in windows.

## Note

There are plenty of instruction like ``mov dword fs:[0], ecx``, this seems related
to the [Win32 Thread Information Block](https://en.wikipedia.org/wiki/Win32_Thread_Information_Block).

In particular see this [answer](https://reverseengineering.stackexchange.com/questions/1911/whats-fs0-doing-and-how-can-i-execute-it-step-by-step).
