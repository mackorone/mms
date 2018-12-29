import sys


def command(*args):
    print(*args)
    print(*args, file=sys.stderr)


CHARS = (
    " !\"#$%&'()*+,-./0123456789:;<=>?"
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
    "`abcdefghijklmnopqrstuvwxyz{|}~"
)


index = 0
for x in range(16):
    for y in range(16):
        text = ""
        for i in range(10):
            text += CHARS[index]
            index = (index + 1) % len(CHARS)
        command("setText", x, y, text)


print("Done")
