import os
import sys

def char_to_quoted_filename(char):
    return '"_' + char + '_.png"'

characters = '0123456789 inf'

fontpath = sys.argv[1]
fontname = fontpath[fontpath.rfind('/') + 1 : fontpath.rfind('.')]

# Options for the imagemagick "convert" command
options = {
    'background': 'none',
    'extent': '16x28',
    'fill': 'gray75',
    'font': fontpath,
    'gravity': 'center',
    'pointsize': '26',
}

# Create each png image individually, centered in a 16x32 pixel block
for char in characters:
    command = ('convert '
        + ' '.join('-' + key + ' ' + value for (key, value) in options.iteritems())
        + ' label:"' + char + '" '
        + char_to_quoted_filename(char))
    os.system(command)

# Join the png images
os.system('convert '
    + ' '.join(char_to_quoted_filename(char) for char in characters)
    + ' +append ' + fontname + '.png')

# Remove the individual png images
for char in characters:
    os.remove(char_to_quoted_filename(char).strip('"'))
