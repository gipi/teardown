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

## Functionalities

It's a work in progress, for now there is the ``UART`` working (but in the
PowKiddy it's not easily accessible), exception handling and ``USB`` is
hopefully in the way.
