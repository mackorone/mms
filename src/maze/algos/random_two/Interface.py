import random


class Interface(object):

    def __init__(self, maze):
        self.__maze = maze
        self.__success = True

    @property
    def width(self):
        return len(self.__maze)
    
    @property
    def height(self):
        return len(self.__maze[0])

    @property
    def success(self):
        return self.__success

    def get_random_float(self):
        return random.random()

    def set_wall(self, x, y, direction, wall_exists):

        # Sanity checks
        if not 0 <= x < self.width:
            print("Error: x value {} is invalid".format(x))
            self.__success = False
            return
        if not 0 <= y < self.height:
            print("Error: y value {} is invalid".format(y))
            self.__success = False
            return
        if len(direction) != 1 or direction not in 'nesw':
            print("Error: direction value '{}' is invalid".format(direction))
            self.__success = False
            return

        # Set the wall value
        self.__maze[x][y][direction] = wall_exists

        # Set the opposing wall value
        if direction == 'n' and y < self.height - 1:
            self.__maze[x][y + 1]['s'] = wall_exists
        if direction == 'e' and x < self.width - 1:
            self.__maze[x + 1][y]['w'] = wall_exists
        if direction == 's' and 0 < y:
            self.__maze[x][y - 1]['n'] = wall_exists
        if direction == 'w' and 0 < x:
            self.__maze[x - 1][y]['e'] = wall_exists
