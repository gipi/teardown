# Open ADEC

The Action's devices have some "upgrade" functionalities available via USB using
some provided binary blobs, the principal ones named something like
"adec_xxx.bin"; they are encrypted and allow to interact with the devices in
order to flash some new firmware or get info about hardware and software.

This project's aim is to provide a general purpose firmware that allows to
interact in a similar way. For now the code is tailored around the ``ATS3603``
but should work (hopefully with minimal modification) for any chip of the family
``ATJ27xx``.

## Build

From this directory run

```
$ make
```

you should find in the ``build/`` directory the output files. With

```
$ make upload
```

it's possible to upload it into an Action's device in ``ADFU`` mode.

It exists a tools, named [``oa``](tools/oa) that can be used to interact with
``open-adec``, for example to dump the flash of the PowKiddy J6 you can do

```
$ tools/oa --device 10d6:10d6 --binary build/open-adec.bin.encrypted flashdump --output spidump.bin
```

this tools uploads the firmware and issues the necessary commands (at the end
reset the board to ``ADFU`` mode again).

The dependency can be installed via ``pip3``

```
$ pip3 install -r tools/requirements.txt
```

## Functionalities

 - [x] ``UART``
 - [ ] ``USB`` (work in progress)
   - [x] custom CBW commands
     - [x] flash dump
     - [ ] memory dump
