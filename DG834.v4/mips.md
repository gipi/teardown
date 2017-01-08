Reset entry: virtual``0xBFC00000`` physical ``0x1FC00000``, Usually has a branch there: ``0x1000xxxx``

The [memory layout](http://www.johnloomis.org/microchip/pic32/memory/memory.html) is pretty strange

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
