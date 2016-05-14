"""
Converts files from the ".MAZ" file format (compatible with maze generator application found here:
https://code.google.com/p/maze-solver/) to the ".maz" file format (compatible with our simulator)
"""

import binascii
import os
import sys

# Note: Use this to automatically convert all files in the current directory
#files = [f for f in os.listdir('.') if '.MAZ' in f]

# Note: Use this to specify the files to convert as command line arguments
files = [f for f in sys.argv[1:] if os.path.isfile(f)]

print 'Converting the following files:', files

for name in files:
    f = open(name, 'r').read()
    out = open(str(name + ".out"), 'w')
    x = 0
    y = 0
    for byte in f:
        walls = str(bin(int(binascii.hexlify(bytearray(byte)), base=16)))[2:].zfill(4)
        print >> out, str(x), str(y), str(walls[3]), str(walls[2]), str(walls[1]), str(walls[0])
        y += 1
        if (y == 16):
            y = 0
            x += 1

print 'DONE!'
