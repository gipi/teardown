#!/usr/bin/env python3
'''
Upload a file from a serial connection.

WARNING: this script assumes that PS1='\$ ' and there aren't log
messages polluting the console.
'''
import os
import sys
import time
import serial
import logging
import tqdm # for the progress bar


# we want to log on file the communications
# and in console the INFO
#logging.basicConfig()
logger_console = logging.getLogger('%s.info' % __name__)
logger_console.setLevel(logging.INFO)
stream = logging.StreamHandler()
logger_console.addHandler(stream)


log_comm = logging.getLogger('%s.comm' % __name__)
log_comm.setLevel(logging.DEBUG)
# create a file handler
handler = logging.FileHandler('serial_upload.log')
handler.setLevel(logging.DEBUG)

logger_console.addHandler(handler)
log_comm.addHandler(handler)




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
    log_comm.debug('> %s' % line)
    s.write(line)

    string_end = b"%s# " % marker
    
    #line_received = s.readline()
    #logger.debug('< %s' % line_received)
    line_received = s.read_until(string_end)
    log_comm.debug('< %s' % line_received)

    if line not in line_received:
        #print(s.readline())
        #raise AttributeError('received:%s vs sent:%s' % (line_received, line))
        pass
    line_received = line_received[:-len(string_end)]
    log_comm.debug('< %s' % line_received)
    return line_received


def usage(progname):
    print(f'usage: {progname} port baud src dst')
    sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) < 5:
        usage(sys.argv[0])

    serial_port_path = sys.argv[1]
    serial_baud = sys.argv[2]
    local_path  = sys.argv[3]
    remote_path = sys.argv[4]

    logger_console.info(' [*] Opening %s port' % serial_port_path)
    connection = serial.Serial(serial_port_path, serial_baud)
    #connection.timeout = .4

    logger_console.info(' [*] Creating empty file %s' % remote_path)
    remote_path = bytes(sys.argv[4], 'utf-8')
    serial_shell_cmd(connection, b'echo -ne "" > \'%s\'' % remote_path)

    CHUNK_SIZE = 64
    local_file_size = os.stat(local_path).st_size

    logger_console.info(' [*] Starting to upload file %s (size %d)' % (local_path, local_file_size))

    with open(local_path, "rb") as output, tqdm.tqdm(total=local_file_size) as progress:
        while True:
            chunk = output.read(CHUNK_SIZE)
            if chunk == b"":
                break
            encoded = get_encoded_repr(chunk)
            serial_shell_cmd(connection, b"echo -en '%s' >> '%s'" % (
                bytes(encoded, 'utf-8'),
                remote_path
            ), True)
            progress.update(CHUNK_SIZE)

