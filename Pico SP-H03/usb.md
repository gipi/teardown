# USB mode

There are three USB mode present in different ways in this device

## Boot mode

This is the mode inside the SoC and is the failsafe for all the boot modes available;
in the specification is described in the following way:

 1. The TCC8900 makes internal SRAM area starts from zero, and copies USB interrupt
    service routine to internal SRAM area.
 2. It waits until USB connection is established.
 3. Once it is connected, host transfers first the parameter for USB loader routine including
    start address, destination address and the amount of data to be transferred (with a unit of
    packet).
 4. The TCC8900 starts communicating between a host PC with fixed amount of data which
    is called as packet. The packet size of TCC8900 is 512 bytes.
 5. At every successful reception of packet, it copies them where the destination address
    pointed, and after all amount of data has been copied, it starts program where the start
    address pointed



## FWDN mode

This is the "normal" firmware upgrade mode, done by the bootloader, ``tccboot``.

## SCSI mode
