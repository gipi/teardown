# ADEC

The ``adec`` is a special module used for "special operation"; since it's
intended to be executed by the [builtin ADFU](../BROM.md#builting-adfu) living in the ``BROM`` must be
encrypted before uploaded.

It must be loaded at the address ``0xb4060000`` and in this way it overwrites
the code from the ``BROM`` with the minimal functionality to read from flash.

**Note:** the actual first ``0x400`` bytes are a initial jump and ``nop``s BUT
it's using that space as stack space!

These below are a couple of internal routines that can be handy if you need only
to patch a little bit of its functionalities:

| Function | address | Description |
|----------|---------|-------------|
| ``usb_cbw_handle()`` | ``0xb40615f8`` | Handle CBW packets |
| ``usb_transmit(uint32_t* buff, size_t size, bool transmit)`` | ``b4060ad4`` | Transmit the contents into buffer via USB |
| ``usb_receive(uint32_t* buff, size_t size, bool receive)`` | ``0xb4060c50`` | Receive a packet and save into the buffer |
| ``handle_afinfo()`` | ``0xb4060da0`` | Get a lot of info about AFI(?) and registers |

## CBW

This module activates new "commands": ``cmd`` must be different from ``0xb0``
and ``arg0 & 0x7f`` can be one of the following

| ``arg0 & 0x7f`` | Description |
|-----------------|-------------|
| ``0x13`` | |
| ``0x16`` | |
| ``0x20`` | |
| ``0x22`` | |
| ``0x23`` | |
| ``0x31`` | |
