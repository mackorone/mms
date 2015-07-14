'''
Checks the parameters in res/parameters and the values in sim/Param.cpp to make
sure that they match. It prints out any inconsistencies between the two.
'''

with open('../res/parameters.xml', 'r') as f:
    lines = [line.strip() for line in f.readlines()]
    # TODO: UP-FOR-GRABS
