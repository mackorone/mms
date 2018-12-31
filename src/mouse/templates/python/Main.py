#!/usr/bin/env python3

import sys

# ----- API -----

class InvalidCommandError(Exception):
    pass

class MouseCrashedError(Exception):
    pass

def command(*args, return_type=None):
    print(*args, file=sys.stderr)
    if return_type:
        response = input()
        if return_type == bool:
            return response == "true"
        return return_type(response)

def mazeWidth():
    return command("mazeWidth", return_type=int)

def mazeHeight():
    return command("mazeHeight", return_type=int)

def wallFront():
    return command("wallFront", return_type=bool)

def wallRight():
    return command("wallRight", return_type=bool)

def wallLeft():
    return command("wallLeft", return_type=bool)

def moveForward():
    response = command("moveForward", return_type=str)
    if response == "crash":
        raise MouseCrashedError()

def turnRight():
    command("turnRight", return_type=str)

def turnLeft():
    command("turnLeft", return_type=str)

def setWall(x, y, direction):
    command("setWall", x, y, direction)

def clearWall(x, y, direction):
    command("clearWall", x, y, direction)

def setColor(x, y, color):
    command("setColor", x, y, color)

def clearColor(x, y):
    command("clearColor", x, y)

def clearAllColor():
    command("clearAllColor")

def setText(x, y, text):
    command("setText", x, y, text)

def clearText(x, y):
    command("clearText", x, y)

def clearAllText():
    command("clearAllText")

def wasReset():
    return command("wasReset", return_type=bool)

def ackReset():
    command("ackReset", return_type=str)

# ----- Algorithm -----

def main():
    while True:
        if not wallLeft():
            turnLeft()
        while wallFront():
            turnRight()
        moveForward()

if __name__ == "__main__":
    main()
