#!/usr/bin/env python3
'''
Upload a file from a serial connection.
'''
import sys
import time
import serial
import logging


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel('DEBUG')



def get_encoded_repr(data):
    line = ""
    for index in range(len(data)):
        line += "\\x%02x" % data[index]

    return line

def serial_shell_cmd(s, cmd, one_more_line=False):
    '''
    Reading a command from a serial port is a little tricky, taking into consideration
    that end of line can be different, and that we don't know for sure what the prompt
    is, so we are adding a trailing random string to recognize the end of it.
    '''
    marker = b'RandomJunk'
    line = b"%s; echo -ne %s\r\n" % (cmd, marker)
    logger.debug('> %s' % line)
    s.write(line)

    string_end = b"%s# " % marker
    
    #line_received = s.readline()
    #logger.debug('< %s' % line_received)
    line_received = s.read_until(string_end)
    logger.debug('< %s' % line_received)

    if line not in line_received:
        #print(s.readline())
        #raise AttributeError('received:%s vs sent:%s' % (line_received, line))
        pass
    line_received = line_received[:-len(string_end)]
    logger.debug('< %s' % line_received)
    return line_received


if __name__ == '__main__':
    serial_port_path = sys.argv[1]
    serial_baud = sys.argv[2]
    local_path  = sys.argv[3]
    remote_path = bytes(sys.argv[4], 'utf-8')

    connection = serial.Serial(serial_port_path, serial_baud)
    #connection.timeout = .4

    serial_shell_cmd(connection, b'echo -ne "" > \'%s\'' % remote_path)

    with open(local_path, "rb") as output:
        while True:
            chunk = output.read(64)
            if chunk == "":
                break
            encoded = get_encoded_repr(chunk)
            serial_shell_cmd(connection, b"echo -en '%s' >> '%s'" % (
                bytes(encoded, 'utf-8'),
                remote_path
            ), True)
