"""
This script implements a client, it waits for the advertising from the server
and then connects to it.

It has only two dependencies, Pillow and PyGame.

After the connection the client setups the communication via several steps in order

 1. security handshake
 2. security options
 3. challenge
 4. client initialization

Completed these steps the client it's finally capable of communicating with
the server and sending specific messages, that is

 1. set pixel format
 2. set encoding
 3. framebuffer request
"""
import argparse
import time
import socket
import logging
import struct
import ssl
import hashlib
import pygame
from PIL import Image
from typing import Tuple


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel('INFO')


UDP_IP = "0.0.0.0"
UDP_PORT = 5901
SSL_PORT = 5900
BANNER = b"RFB 003.008\n"


def sha256me(data: bytes) -> bytes:
    m = hashlib.sha256()
    m.update(data)
    return m.digest()


def challengeme(userid: bytes, timestamp: bytes):
    hash_userid = sha256me(userid)

    data = timestamp + hash_userid

    return sha256me(data)


def waiting_advertize() -> Tuple[bytes, str]:
    """The device advertizes itself on broadcast."""
    logger.info("waiting for device's advertizing")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    datagram, (addr, port) = sock.recvfrom(0x8 + 0x4 + 0x20)
    logger.info("received advertizing from: %s" % addr)
    logger.debug(f"{datagram=}")

    raw_timestamp = datagram[:8]
    hash_length = struct.unpack("I", datagram[8:8 + 4])[0]
    hash_email = datagram[8 + 4:]

    timestamp = struct.unpack("!Q", raw_timestamp)[0]

    # check with bash 'echo $((${EPOCHREALTIME/,/} / 1000))'
    logger.debug("timestamp from server: %d" % timestamp)

    # TODO: check hash email
    logger.debug(f"{hash_email=}")

    return raw_timestamp, addr


def ssl_connect(hostname: str, timestamp: bytes, userid: bytes, resolution: Tuple[int, int]):
    """Connect to the device and step through the state machine.

    NOTE: the return values from the server are not always the same
    for the right answer (can be 0 or 1, 4 bytes or a single byte)."""
    context = ssl.create_default_context()

    # we trust you buddy
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    with socket.create_connection((hostname, SSL_PORT)) as sock:
        with context.wrap_socket(sock, server_hostname=hostname) as ssock:
            logger.debug(ssock.version())

            print(ssock.getpeercert())

            # first step is receiving and sending the banner
            logger.debug("# banner")
            banner = ssock.recv(len(BANNER))

            logger.debug("banner from server: %s" % banner)

            if banner != BANNER:
                raise ValueError("we were expecting '{}' but we received {}".format(BANNER, banner))

            ssock.send(BANNER)

            ret = ssock.recv(1)

            logger.debug("ret from banner: %s" % ret)

            if ret != b'\x01':
                raise ValueError("we weren't expecting '{}'".format(ret))

            # security options
            logger.debug("# security options")

            securityOptions = ssock.recv(4)

            logger.debug("security options from server: %s" % securityOptions)

            ssock.send(bytes([100]))  # it expects a single byte

            ret = ssock.recv(4)  # some return 4 bytes other only 1

            logger.debug("ret from security options: %s" % ret)

            if ret != b'\x00\x00\x00\x00':
                raise ValueError("we weren't expecting '{}'".format(ret))

            logger.debug("# challenge")
            # now time for the challenge
            # first we send the length of the challenge (0x20 bytes)
            ssock.send(b'\x00\x00\x00\x20')

            challenge = challengeme(userid, timestamp)

            logger.debug(f"{challenge=}")

            # then the SHA256 of the userid
            ssock.send(challenge)

            ret = ssock.recv(1)

            logger.debug("ret from challenge: %s" % ret)

            if ret != b'\x00':
                raise ValueError("Challenge failed: we weren't expecting '{}' wrong auth code".format(ret))

            # now it's time for initialization
            ssock.send(b'\xaa')  # this is probably not used for anything

            width = struct.unpack(">H", ssock.recv(2))[0]

            logger.debug(f"{width=}")

            height = struct.unpack(">H", ssock.recv(2))[0]
            logger.debug(f"{height=}")

            """
            what's follows are these bytes of unknown usage
            
            \x10 \x10 \x00 \x01  <--- init
            \x00\x1f \x00\x3f \x00\x1f
            \x0b \x05 \x00
            \x00 \x00 \x00'
            """
            params = ssock.recv(0x10)

            logger.debug(f"{params=}")

            """
            then we receive a string that indicates the starting of the sharing
            
            b'\x00\x00\x00\x0e'  <--- length of string
            b'reMarkable rfb'
            """

            ss_banner_length = struct.unpack("I", ssock.recv(4))[0]
            ss_banner = ssock.recv(ss_banner_length)

            logger.info("sharing started with banner '{}'".format(ss_banner))

            pygame.init()
            pygame.display.init()
            screen = pygame.display.set_mode(resolution)

            logger.debug("""asking for a pixel format that we can use
             GIMP is able to import it as 8bit grayscale
             or you can use ImageMagick's 'convert -size 1404x1872 -depth 8 GRAY:/tmp/mmiao.dump /tmp/ss.png'""")
            # first of all the code to set the encoding
            # since the default one sends only zeroes
            ssock.send(b'\x00')
            # change params (the first byte indicates how many bytes for pixel I think)
            ssock.send(b'\x08' + params[1:0xd])

            # now we are ready to ask for the screen and obtain it
            run = True
            while run:
                logger.info("requesting framebuffer region")

                # first of all the code to request the framebuffer
                ssock.send(b'\x03')
                # now we are sending 9 bytes to identify the rectangle of the screen
                # we are interested to receive
                ssock.send(b'\x00')  # override
                ssock.send(b'\x00\x00')  # x = 0
                ssock.send(b'\x00\x00')  # y = 0
                ssock.send(struct.pack(">H", width))
                ssock.send(struct.pack(">H", height))

                # first of all we receive the header
                header = ssock.recv(0x10)
                # FIXME: check that the region makes sense
                length = width * height

                image_data = b''
                while length > 0:
                    data = ssock.recv(length)
                    length -= len(data)

                    image_data += data

                image = Image.frombytes('L', (width, height), image_data)

                image_raw = image.resize(resolution).convert("RGBA").tobytes("raw", "RGBA")
                pygame_surface = pygame.image.fromstring(image_raw, resolution, 'RGBA')
                screen.blit(pygame_surface, (0, 0))
                pygame.display.update()

                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        run = False
                    if event.type == pygame.KEYDOWN:
                        print(pygame.key.name(event.key))

                time.sleep(1)


def argparse_resolution(value):
    width, height = tuple(value.split("x"))

    return int(width, 0), int(height, 0)


def parse_args():
    args = argparse.ArgumentParser(description='share your screen')

    args.add_argument('--auth', required=True)
    args.add_argument(
        '--resolution',
        type=argparse_resolution,
        required=True,
        help="resolution indicated as <widht>x<height>")

    return args.parse_args()


if __name__ == '__main__':
    args = parse_args()

    timestamp, addr_device = waiting_advertize()
    ssl_connect(addr_device, timestamp, args.auth.encode('utf8'), args.resolution)
