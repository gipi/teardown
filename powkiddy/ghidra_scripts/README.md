# Ghidra scripts

This directory contains a couple of scripts that can be handy to use with
reversing the PowKiddy

 - [syscall resolver](resolve_syscalls.py): using the ``v1`` register
   generate the correct call to the ucos-II's syscall as described by the
   ``actsemi.c``. You need to load the syscall calling convention [``XML`` file](syscall.cspec.xml) using ``Edit >
   Options for <program name> > Specification Extensions``.
 - [ZLF decoder](ZLF.py): decompress the segments to make a usable ``ELF`` file;
   it generates a new memory block for each entry in the ``__bcomprtab``
   section.
