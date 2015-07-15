# Font Stash: Dynamic font glyph cache for OpenGL

Font Stash enables easy string rendering in OpenGL applications. It supports truetype fonts and UTF-8 encoded localized strings. All glyphs are cached in OpenGL texture atlases. Font rasterization is done using Sean Barrett's [stb_truetype.h](http://nothings.org/).

Font Stash was originally created and [published](http://digestingduck.blogspot.com/2009/08/font-stash.html) by [Mikko Mononen](http://digestingduck.blogspot.com).

## Major changes to the original version

* Bitmap font support
* Truetype font loading from memory
* Added ability to load an arbitrary number of fonts (not just four)
* Use multiple OpenGL textures for glyph caching (not just one)

## Road map

* Freetype2 support (for glyph outlines)

## Screenshot

![Screenshot](https://github.com/akrinke/Font-Stash/wiki/screenshot.png)

## Usage

```c
#include "fontstash.h"

...
/* create a font stash with a maximum texture size of 512 x 512 */
struct sth_stash* stash = sth_create(512, 512);
/* load truetype font */
int droid = sth_add_font(stash, "DroidSerif-Regular.ttf");
/* position of the text */
float x = 10, y = 10;
...
/* draw text during your OpenGL render loop */
sth_begin_draw(stash);
/* position: (x, y); font size: 24 */
sth_draw_text(stash, droid, 24.0f, x, y, "Hello world!", &x);
/* now, the float x contains the x position of the next char */
sth_end_draw(stash);
... 

```

## Bitmap fonts

Bitmap fonts can be generated using the [AngelCode Bitmap Font Generator](http://www.angelcode.com/products/bmfont/). It's a Windows program that runs under Linux using Wine. This program generates one or more image files and a text file (*.fnt) containing the bounding box coordinates of all individual glyphs. These coordinates are used to reference the glyphs using Font Stash.

## License

The files fontstash.c, fontstash.h and main.c are licensed unter the zlib license:

    Copyright (c) 2011 Andreas Krinke andreas.krinke@gmx.de
    Copyright (c) 2009 Mikko Mononen memon@inside.org

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

For UTF-8 decoding, Font Stash uses Björn Höhrmann's [Flexible and Economical UTF-8 Decoder](http://bjoern.hoehrmann.de/utf-8/decoder/dfa/) (included in fontstash.c).
His code is licensed under the MIT license:

    Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
    THE SOFTWARE.
    
Finally, Font Stash uses Sean Barrett's public domain truetype font rasterizer [stb_truetype.h](http://nothings.org/).

