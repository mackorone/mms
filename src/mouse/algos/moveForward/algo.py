import sys
import time

print('turnRight', file=sys.stderr)
input()

count = 10

for i in range(count):
    print('moveForward', file=sys.stderr)

sys.stderr.flush()

for i in range(count):
    input()
