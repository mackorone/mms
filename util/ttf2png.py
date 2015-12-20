import os
import sys
import string

# TODO: MACK - Vertical alignment of the text :/
# TODO: MACK - Refactor this to use format strings, methods, etc.
# TODO: MACK - error message is annoying

def char_to_quoted_png_name(char):
    return '"_' + str(ord(char)) + '_.png"'

def char_to_txt_file_name(char):
    last_slash_index = sys.argv[0].rfind('/') + 1
    return sys.argv[0][:last_slash_index] + 'chars/{}.txt'.format(ord(char))

characters = open('chars.txt').read().rstrip() # Strip off the newline

if (len(sys.argv) < 2):
    print('Usage: python ttf2png.py <TTF-FILE>')
    sys.exit(1)
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
    'trim': '',
}

# Create each png image individually, centered in a 16x32 pixel block
for char in characters:
    command = (
        'convert '
        + ' '.join('-' + k + ' ' + v for (k, v) in options.items())
        + ' ' + 'label:@' + char_to_txt_file_name(char)
        + ' ' + char_to_quoted_png_name(char)
    )
    os.system(command)

# Join the png images
os.system(
    'convert '
    + ' '.join(char_to_quoted_png_name(char) for char in characters)
    + ' +append ' + fontname + '.png'
)

# Remove the individual png images
for char in characters:
    os.remove(char_to_quoted_png_name(char).strip('"'))
