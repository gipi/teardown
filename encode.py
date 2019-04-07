#!/usr/bin/env python3
# encoding: utf-8
import sys


if __name__ == '__main__':
    binary_path = sys.argv[1]
    output_path = sys.argv[2]

    contents = None
    with open(binary_path, "rb") as b:
        contents = b.read()

    #r_contents = repr(contents)[2:-1] # remove "b'...'"
    # create a list of sequences of char each containing 10 "\x"
    lines = []
    line = ""
    for index in range(len(contents)):
        line += "\\\\x%02x" % contents[index]
        if index % 9 == 0:
            lines.append(line)
            line = ""

    with open(output_path, "w") as o:
        for line in lines:
            o.write(line)
            o.write("\n")

