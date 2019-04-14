#!/usr/bin/env python3
'''
Dump a file from a serial connection.

FIX: the file dumped has '\n' -> '\r\n'
FIX: make an iterable the dump with a progress bar
'''
import sys
import serial


def serial_shell_cmd(s, cmd):
    '''
    Reading a command from a serial port is a little tricky, taking into consideration
    that end of line can be different, and that we don't know for sure what the prompt
    is, so we are adding a trailing random string to recognize the end of it.
    '''
    marker = b'RandomJunk'
    line = b"%s; echo -ne %s\r\n" % (cmd, marker)
    s.write(line)
    line_received = s.readline()

    if line_received != line:
        raise AttributeError('received:%s vs sent:%s' % (line_received, line))
    string_end = b"%s# " % marker
    r = s.read_until(string_end)
    return r[:-len(string_end)]


if __name__ == '__main__':
    serial_port_path = sys.argv[1]
    serial_baud = sys.argv[2]
    remote_path = sys.argv[3]
    local_path  = sys.argv[4]

    connection = serial.Serial(serial_port_path, serial_baud)
    #connection.timeout = 5

    file_contents = serial_shell_cmd(connection, b"cat '%s'" % bytes(remote_path, 'utf-8'))

    with open(local_path, "wb") as output:
        output.write(file_contents)
