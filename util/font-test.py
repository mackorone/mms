import sys


def command(*args):
    print(*args)
    print(*args, file=sys.stderr)


def request(*args):
    command(*args)
    return int(input())


CHARS = (
    " !\"#$%&'()*+,-./0123456789:;<=>?"
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
    "`abcdefghijklmnopqrstuvwxyz{|}~"
)


width = request("mazeWidth")
height = request("mazeHeight")
index = 0
for x in range(width):
    for y in range(height):
        text = ""
        for i in range(10):
            text += CHARS[index]
            index = (index + 1) % len(CHARS)
        command("setText", x, y, text)


print("Done")
