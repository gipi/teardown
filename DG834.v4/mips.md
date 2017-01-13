# MIPS

## Memory layout

Reset entry: virtual``0xBFC00000`` physical ``0x1FC00000``, Usually has a branch there: ``0x1000xxxx`` (see [MIPS bootstrapping](http://www.nulltrace.org/2013/04/mips-bootstrapping.html))

The [memory layout](http://www.johnloomis.org/microchip/pic32/memory/memory.html) is pretty strange: has four sections

 - ``kseg2`` 0xc000.0000 - 0xffff.ffff
 - ``kseg1`` 0xa000.0000 - 0xbfff.ffff
 - ``kseg0`` 0x8000.0000 - 0x9fff.ffff
 - ``kuseg`` 0x0000.0000 - 0x7fff.ffff

There are logical reasons for the existence of the memory segments:

 1. Caches in MIPS need to be initialized by boot code, (unlike x86 caches which are initialized by the hardware).
 2. The memory management unit (MMU) in embedded systems is optional, so it is useful to have explicit physical memory regions reserved for the kernel, and not accessible by user mode code.

Here is what the regions are used for:

- KSEG1 addresses are uncached and are not translated by the MMU. KSEG1 is the only memory region that can be used at reset because the MMU and caches on MIPS CPUs must be configured by the boot code, which must be placed in KSEG1. 

- KSEG0 provides an address region for the kernel that is cached, but not mapped by the MMU.

- KSEG2 is used for kernel mode code that is mapped by the MMU and cached.

- KUSEG is used for user mode code that is mapped by the MMU and cached.

## Assembly

The [assembly](http://logos.cs.uic.edu/366/notes/mips%20quick%20tutorial.htm) [reference](http://www.mrc.uidaho.edu/mrc/people/jff/digital/MIPSir.html).

[Seems](http://www.linux-mips.org/wiki/SB1) that this kind of ``MIPS`` is ``sb1``, a highperformance implementation of the MIPS64 architecture.

Be aware of the [branch delay slot](http://stackoverflow.com/questions/3807480/weird-mips-assembler-behavior-with-jump-and-link-instruction)

```
MIPS has explicit pipeline hazards; the instruction immediately following
a branch or jump instruction will always be executed (this instruction is
sometimes referred to as the "branch delay slot").
```


## Assembly

### Build a 32-bit value

It's possible in a two steps process: first use the ``lui`` instruction that loads
the upper part of a register with a 16-bit value, zero extending the lsb.

After that you can use the ``addi`` that has the problem of sign-extends so
it's possible tu use ``ori`` for negative numbers.

Links
-----

 - http://shadow-file.blogspot.it/2013/05/running-debian-mips-linux-in-qemu.html
 - https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Mips/load32.html
