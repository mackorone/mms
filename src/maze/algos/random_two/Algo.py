class Algo(object):

    def generate(self, interface):
        for i in range(interface.width):
            for j in range(interface.height):
                for direction in 'nesw':
                    interface.set_wall(
                        i,
                        j,
                        direction,
                        interface.get_random_float() < .4
                    )
