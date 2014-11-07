# Converts from the MAZ file format (compatible with maze generator) to the maz file
# format (compatible with out simulator)

import binascii
import os

files = [f for f in os.listdir(".") if ".MAZ" in f]
for name in files:
    f = open(name, 'r').read()
    out = open(str(name[:-3] + "maz").lower(), 'w')
    x = 0
    y = 0
    for byte in f:
        walls = str(bin(int(binascii.hexlify(bytearray(byte)), base=16)))[2:].zfill(4)
        print >> out, str(x), str(y), str(walls[3]), str(walls[2]), str(walls[1]), str(walls[0])
        y += 1
        if (y == 16):
            y = 0
            x += 1
    os.system("rm " + name)
