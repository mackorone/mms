Release
=======
- Clean up mouse algo template
    - Add Python bindings
- Publish installation and usage guides
    - Update the wiki
    - Make some video tutorials
    - Page for common error message for failed commands
- Fix UI updates on mouse algo start/stop
- Disable some UI buttons when algo completes
- Fix the FPS bugs
- Respond with "bump" (or something) if can't move forward
- Add a "new algo" wizard to make it easy to bootstap a new algo
    - Auto-populate build and run commands

Wishlist
========
- API call counts
    - A table of API method to # of times called
- Update the build and run status when the algorithm changes
- Monitor algo directories using FileSystemWatcher
    - Make it obvious when the user should re-build their algo
- Look into using vsync so the graphics don't tear
- Make a system for quickly checking stats on many mazes
    - solved or not
    - how many steps
    - ave # of steps

Cleanup
=======
- Add unit tests
- Remove superfluous include statements
- MacOS retina https://github.com/vispy/vispy/issues/99
- Figure out a better interface for input buttons
- Don't actually log within mouse interface - surface errors a different way
- Call all "algo stop" functions during Window initialization
    - Deduplicate initialization with "stop algo" code (and "failed algo" code)
- Make a note about why we write NO_ACK_STRING for some dispatched commands
    - It's a speed optimization, similar to UDP vs. TCP
- Kill the SimUtilities class, along with other platform dependent code
- Replace push_back() with append()
- Lint the codebase
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
- Rename the whole texture vs polygon thing
- QSet instead of QVector in some places
- Use tr() on all user-facing strings to make text translatable
- Use uniform syntax for Signal/slot connections
- Rename Tile to Cell
- Fix up so that -Wall -Werror -pedantic-error flags pass
    - Wall: Show all warning messages
    - Werror: Fail compilation on warnings
    - pedantic-errors: Flag even the most pedantic of errors
- Put MazeView into MazeGraphic (it should all be encapsulated in there)
- Pointers to wall polygons in each tile, save space by not breaking up walls/corners into multiple tiles
- Optimize for space in the TEXTURE_CPU_BUFFER and GRAPHIC_CPU_BUFFER - Don't repeat vertices
- Rename TriangleTexture and VertexTexture
- Convert primitive types to GL types (or vice versa)
- Change "bool foo(false)" to "bool foo = false" for primitive - they look like function calls
