Release
=======
- Clean up mouse algo template
    - Add Python bindings
- Publish installation and usage guides
    - Update the wiki
    - Make some video tutorials
    - Page for common error message for failed commands
- Fix pause button
- Add reset button
- Fix UI updates on mouse algo start/stop
- Disable pause button when algo completes
- Improve FPS bugs
- Respond with "bump" (or something) if can't move forward
- Update the build and run status when the algorithm changes
- Add a "new algo" wizard to make it easy to bootstap a new algo
    - Auto-populate build and run commands

Cleanup
=======
- Add unit tests
- Lint the codebase
- Remove superfluous include statements
- Remove all unnecessary settings classes
- Look into using vsync so the graphics don't tear
- MacOS retina https://github.com/vispy/vispy/issues/99
- Don't actually log within mouse interface - surface errors a different way
- Call all "algo stop" functions during Window initialization
    - Deduplicate initialization with "stop algo" code (and "failed algo" code)
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
- QSet instead of QVector in some places
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

Wishlist
========
- Make a system for quickly checking stats on many mazes
    - solved or not
    - how many steps
    - ave # of steps
