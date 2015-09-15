import os
import sys

font = sys.argv[1]
chars = sys.argv[2]

os.system('convert -background none -fill gray75 -font ' + font + ' -pointsize 26 '
' -extent ' + str(16 * len(chars)) + 'x32 label:"' + chars + '" ' + chars + '.png')
