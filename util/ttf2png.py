import os
import sys
import string

def end_of_path_index(full_path):
    return full_path.rfind('/') + 1

def get_path(full_path):
    return full_path[:end_of_path_index(full_path)]

def get_name(full_path):
    return full_path[end_of_path_index(full_path):]

def ttf2png(chars_path, font_path, dest_path):

    options = {
        'background': 'none',
        'fill': 'gray75',
        'font': font_path,
        'geometry': '{}x'.format(16 * len(open(chars_path).read())),
        'gravity': 'center',
        'pointsize': '32', # This is somewhat arbitrary
    }

    command = 'convert {options} {label} {dest_path}'.format(
        options = ' '.join(
            '-{} {}'.format(k, v) for (k, v) in options.items()
        ),
        label = 'label:@{}'.format(chars_path),
        dest_path = dest_path,
    )

    os.system(command)

if __name__ == '__main__':

    if (len(sys.argv) < 2):
        print('Usage: python ttf2png.py <TTF-FILE>')
        sys.exit(1)

    chars_path = get_path(sys.argv[0]) + 'chars.txt'
    font_path = sys.argv[1]
    font_name = get_name(font_path)

    ttf2png(chars_path, font_path, font_name.replace('.ttf', '.png'))
