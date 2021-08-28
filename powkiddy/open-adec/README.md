# Open ADEC

The Action's devices have some "upgrade" functionalities available via USB using
some provided binary blobs, the principal ones named something like
"adec_xxx.bin"; they are encrypted and allow to interact with the devices in
order to flash some new firmware or get info about hardware and software.

This project's aim is to provide a general purpose firmware that allows to
interact in a similar way.

The supported devices are the following

| Device      | SoC         | config         | load address   |UART | USB | Storage |
|-------------|-------------|----------------|----------------|-----|-----|---------|
| PowKiddy J6 | ``ATS3603`` | ``powkiddy``   | ``0xb4061000`` | Y   | Y   | Y       |
| Mirascreen  | ``AM8252``  | ``mirascreen`` | ``0xb4043000`` | Y   | Y   | Y       |

## Build

From this directory run

```
$ make BOARD=<board>
```

you should find in the ``build_<board>/`` directory the output files. For now
the supported boards are ``powkiddy`` and ``mirascreen``.

It exists a tools, named [``oa``](tools/oa) that can be used to interact with
``open-adec``, for example to dump the flash of the PowKiddy J6 you can do

```
$ make BOARD=powkiddy
$ tools/oa --device 10d6:10d6 --address 0xb4041000 --binary build_powkiddy/open-adec_powkiddy.bin.encrypted flashdump --output spidump.bin
```

this tools uploads the firmware and issues the necessary commands (at the end
reset the board to ``ADFU`` mode again).

The dependency can be installed via ``pip3``

```
$ pip3 install -r tools/requirements.txt
```

(it uses the ``adfu`` module contained in a subdirectory of this repository).

## Functionalities

 - [x] ``UART``
 - [x] ``USB``: reusing the endpoints as configured by the BROM
   - [x] custom CBW commands
     - [x] flash dump
     - [x] memory dump
     - [x] stacktrace


## Porting

This should be considered as a skeleton to implement basic functionalities in
order to interact with these kinds of devices, it's not intended to be in future
a bootloader with drivers support, for that is more useful (in my opinion) port
something like ``u-boot``.

if you are interested in porting a new board you have to write a new file
describing the device into ``config/``, like the following

```
SOC=AM8252
BUILD_USB=y
USB_ID=1de1:1205
BUILD_EXCEPTIONS=y

CONFIG_IRAM_SIZE=0x8000
# the space before is used by the BROM's USB routines
CONFIG_ENTRY_ADDRESS=0xb4043000
CONFIG_STACK_START_ADDRESS=0xb4048000
# the NAND is 1GB
CONFIG_NAND_SIZE=1073741824
```

All what's starting with ``CONFIG_something=foobar`` will end into
``include/config.h`` as ``#define something foobar`` and will be used by the
``C``/``asm`` and the linker. The ``SOC`` variable will indicate the
subdirectory inside ``src/socs/`` where to look for the architecture dependent
files (``brom.c`` and ``hal.c``).

The hard part is finding information in order to write these files: the main
approach is to reverse engineer the available ``adec`` for the board in question
and from that discover a way to dump the boot ROM.

Generally the BROM has a way of reading the storage of the board itself, mapping
that part "locally" and then calling a couple of times the functions defined
there to read the flash or whatever is needed to boot the board. To see an
example see the [``brom.c``](src/socs/ats3603/brom.c) for the PowKiddy.
