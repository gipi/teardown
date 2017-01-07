# Vodafone Gateway

## Serial

The pinout is

 - TX
 - VCC
 - GND
 - 
 - RX (near the capacitor)

Connecting to the serial gives nothing, only some random
messages

```
*** gStartRxDesc[0] = 0xA02D3000 
*** gBufferSizeBytes = 1280 
*** gStartTxDesc[0] = 0xA02D4000 
hal6358PcmInit 260 nextTxDesc = 0xA02D4000 
hal6358PcmInit 260 nextTxDesc = 0xA02D4008 
hal6358PcmInit 264 Ownership for TX desc not set. Use this buffer. 
PERF->IrqMask   = 0xA00A2CA4 
PERF->IrqMask1  = 0x00800000
```

## JTAG

The pinout is the following

```
 10 GND      9  TDI
 8  nTRST??  7  (??)
 6  nSRTT??  5  TMS
 4  VCC      3  TDO
 2  GND      1  TCK
```

The orientation of the pinout is found checkig the row with the
two ``GND`` for contuinity.

