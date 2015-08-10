'''
Checks the parameters in res/parameters and the values in sim/Param.cpp to make
sure that they match. It prints out any inconsistencies between the two.
'''

import re
import inspect, os

utilPath = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

with open(utilPath + '/../res/parameters.xml', 'r') as xml, open(utilPath + '/../src/sim/Param.cpp') as cpp:

    xml_regex = re.compile('<\S+>.+</\S+>')
    xml_tags = xml_regex.findall(xml.read())
    xml_dict = dict()
    for tag in xml_tags:
        tokens = re.split('[<|>]', tag)
        xml_dict[tokens[1]] = tokens[2]

    # TODO: upforgrabs
    # This script is broken in two ways. First, the default value may be
    # followed by a comma and not just a parenthesis. Second, the
    # initialization of a single variable may span two lines. The cpp_regex and
    # other lines of logic don't account for either of these. Fix this.

    cpp_regex = re.compile('parser\.get.+IfHas.+\(\"\S+\".*\)')
    cpp_lines = cpp_regex.findall(cpp.read())
    cpp_dict = dict()
    for line in cpp_lines:
        tokens = re.split(',', line)
        name = re.findall('\"\S+\"', tokens[0])[0][1:-1]
        value = re.findall('\S*\W*\)', tokens[1])[0][:-1].strip().strip('"')
        cpp_dict[name] = value

    for pair in xml_dict.iteritems():
        if pair[0] not in cpp_dict:
            print('"' + pair[0] + '" was found in XML but not in CPP')
        elif pair[1] != cpp_dict[pair[0]]:
            print('"' + pair[0] + '" has value "' + pair[1] + '" in XML but "' + cpp_dict[pair[0]] + '" in CPP')
            del cpp_dict[pair[0]] # So we don't get duplicate printouts
            
    for pair in cpp_dict.iteritems():
        if pair[0] not in xml_dict:
            print('"' + pair[0] + '" was found in CPP but not in XML')
        elif pair[1] != xml_dict[pair[0]]:
            print('"' + pair[0] + '" has value "' + pair[1] + '" in CPP but "' + xml_dict[pair[0]] + '" in XML')
