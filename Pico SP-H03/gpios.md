# GPIOs recovery

In order to boot a system or troubleshooting what is wrong with the
actual not booting system, we need to find out where the serial port
is, to do this we need to recovery the position o the board of the
GPIOs.

## Decoding

We are going to use ``sigrok``

 - [Protocol decoder API/Queries](https://sigrok.org/wiki/Protocol_decoder_API/Queries)
 - [PWM decoder source code](https://sigrok.org/gitweb/?p=libsigrokdecode.git;a=blob;f=decoders/pwm/pd.py;h=97704450b7701f5a01387f234eea6e9d16cc0a8e;hb=HEAD)

