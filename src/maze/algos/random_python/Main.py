import random
import sys

from Algo import Algo
from Interface import Interface
from Printer import Printer


def main():

    # Print the usage
    if not 3 <= len(sys.argv) <= 4:
        print("Usage: python Main.py <WIDTH> <HEIGHT> [<SEED>]")
        return

    # Read the width and height args
    width = int(sys.argv[1])
    height = int(sys.argv[2])
    if width <= 0 or height <= 0:
        print("Error: <WIDTH> and <HEIGHT> must be positive integers")
        return

    # Read the seed arg
    if len(sys.argv) == 4:
        seed = int(sys.argv[3])
        if seed < 0:
            print("Error: <SEED> must be a non-negative integer")
            return
        random.seed(seed)

    # Generate an empty maze
    maze = [
        [
            {
                c: False for c in 'nesw'
            }
            for j in range(height)
        ]
        for i in range(width)
    ]

    interface = Interface(maze)
    Algo().generate(interface)

    if interface.success:
        Printer.print_to_stderr(maze)


if __name__ == '__main__':
    main()
