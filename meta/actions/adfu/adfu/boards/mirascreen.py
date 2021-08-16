"""
The Mirascreen device's ADEC has two commands, mimicking the ADFU original ones.
"""
import logging
from hexdump import hexdump
from .. import cbw


logger = logging.getLogger(__name__)


ADEC_MEMORY_CMD = 0x5
ADEC_MEMORY_READ_ARG2 = 0x80
ADEC_PACKET_SIZE = 0x200


def adec_memory_dump(ep_r, ep_w, addr, length: int):
    logger.info(f"dump {length} bytes of memory from address {addr:08x}")

    cbw.cbw_send(
        ep_w,
        ADEC_MEMORY_CMD,
        0x00,
        addr,
        length,
        ADEC_MEMORY_READ_ARG2,
    )

    data = ep_r.read(length)

    logger.debug('response:\n' + hexdump(data, result='return'))

    cbw.cbw_read_response(ep_r)

    return data
