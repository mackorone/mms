Release
=======
- Clean up mouse algo template
    - setWall and clearWall
    - Add Python bindings
- Publish installation and usage guides
    - Update the wiki
    - Make some video tutorials
    - Page for common error message for failed commands
- Panel is too large, set max size for maze file combo box
- Reset button causes mack algo to fail ... why?

Cleanup
=======
- Run and build command should be lists of strings, not single string
- Add unit tests
- Lint the codebase
- Remove superfluous include statements
- Move mouse-related state from window class into mouse class 
- Look into using vsync so the graphics don't tear
- MacOS retina https://github.com/vispy/vispy/issues/99
- Make a note about why we write NO_ACK_STRING for some dispatched commands
    - It's a speed optimization, similar to UDP vs. TCP
- Kill SimUtilities
- Get rid of unnecessary QString wrapping, like QString(<SOME-QSTRING>)
- Use keyword explicit on one argument constructors
- Mark some default constructors as ASSERT NEVER RUNS
- Make methods const where appropriate
- Use const references in for-loops where possible
- Update the codebase to make use of implicit sharing
    - http://doc.qt.io/qt-5/implicit-sharing.html#implicitly-shared
    - Pass other arguments by reference when possible
- Replace lambdas with normal methods (easy to reason about)
    - Audit pass-by-reference or pass-by-value in lambdas
- Make classes as QObjects
    - Make sure all QObjects have parents
- Make sure all QObject membors are stored on the heap
- QSet instead of QVector in some places
- QStringLiteral instead of QString
- Use tr() on all user-facing strings to make text translatable
- Use uniform syntax for signal/slot connections
- Fix up so that -Wall -Werror -pedantic-error flags pass
    - Wall: Show all warning messages
    - Werror: Fail compilation on warnings
    - pedantic-errors: Flag even the most pedantic of errors
- Put MazeView into MazeGraphic (it should all be encapsulated in there)
- Pointers to wall polygons in each tile, save space by not breaking up walls/corners into multiple tiles
- Optimize for space in the TEXTURE_CPU_BUFFER and GRAPHIC_CPU_BUFFER - Don't repeat vertices
- Convert primitive types to GL types (or vice versa)
- Change "bool foo(false)" to "bool foo = false" for primitive - they look like function calls
- Rename Tile to Cell
- Rename the whole texture vs polygon thing
- Rename TriangleTexture and VertexTexture
- Position vs. location vs. coordinate, direction vs. rotation vs. angle
- Shrink icon file size

Wishlist
========
- Add a "new algo" wizard to make it easy to bootstap a new algo
    - Auto-populate build and run commands
- Run "communication" tab (sim input and responses)
- FPS optimizations
    - Use one buffer for static attributes, one for dynamic
        - Static: tile xy-coords, tile corner color, texture v-coords
        - Dynamic: tile base color, tile wall alpha, texture xyu-coords
    - Ensure data in VBOs is aligned properly
    - Memmap for better attribute streaming
    - Use unsigned char for texture v-coord
    - Use index buffer objects
- Make a system for quickly checking stats on many mazes
    - solved or not
    - how many steps
    - ave # of steps
