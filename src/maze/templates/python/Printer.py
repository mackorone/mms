import sys


class Printer(object):

    @staticmethod
    def _eprint(*args, **kwargs):
        print(*args, file=sys.stderr, **kwargs)

    @staticmethod
    def print(maze):

        # The characters to use in the file
        post = '+'
        space = ' '
        vertical = '|'
        horizontal = '-'

        # A blank line, and a list of all lines to be written
        blankLine = [space for x in range(4 * len(maze) + 1)]
        upsideDownLines = []
        upsideDownLines.append(blankLine.copy());

        # For all tiles in the maze
        for i in range(len(maze)):
            for j in range(len(maze[i])):

                # Insert more lines if necessary
                if len(upsideDownLines) <= 2 * j + 1:
                    upsideDownLines.append(blankLine.copy())
                    upsideDownLines.append(blankLine.copy())

                # Insert posts at the boundaries
                left  = 4 * (i + 0)
                right = 4 * (i + 1)
                up    = 2 * (j + 1)
                down  = 2 * (j + 0)
                upsideDownLines[down][left]  = post
                upsideDownLines[down][right] = post
                upsideDownLines[up][left]    = post
                upsideDownLines[up][right]   = post

                # Insert walls if they exist
                if maze[i][j]['n']:
                    for k in range(3):
                        upsideDownLines[up][left + 1 + k] = horizontal
                if maze[i][j]['s']:
                    for k in range(3):
                        upsideDownLines[down][left + 1 + k] = horizontal
                if maze[i][j]['e']:
                    upsideDownLines[down + 1][right] = vertical
                if maze[i][j]['w']:
                    upsideDownLines[down + 1][left] = vertical

        # Flip the lines so that they're right side up
        rightSideUpLines = []
        for i in range(len(upsideDownLines) - 1, -1, -1):
            rightSideUpLines.append(upsideDownLines[i])

        # Print the lines to stderr
        for line in rightSideUpLines:
            Printer._eprint(''.join(line))
