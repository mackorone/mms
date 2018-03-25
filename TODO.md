Features
========
- Add a "new algo" wizard to make it easy to bootstap a new algo
- Support more maze file formats
    - .MAZ, .mz2, https://code.google.com/p/maze-solver/wiki/MazeFileFormats
    - JSON, https://github.com/bblodget/MicromouseSim/tree/gh-pages/mazes_json
- Make it easier to edit the simulator parameters
    - Write and read from persistant storage...
    - The values should be re-loaded on the fly
- Add Python bindings for mouse algorithms
- Ensure that no new instances of wheels/sensors are created during algo execution
- Improve the config dialog field to support double, int, bool
- Make wall-length and wall-width constants
- Make DT configurable
- Investigate why random seed isn't working as expected

Wishlist
========
- Maze "Save As..."
- Save the most recent maze
- API call counts
    - A table of API method to # of times called
- Ad hoc maze rotation and mirroring
- Toggle algorithm output line wrap
- Surface information about why a maze is valid/invalid
    - Perhaps a pane that pops up on mouse over
- Support continuous algorithms
- Display the dynamic mouse algorithm options
- Update the build and run status when the algorithm changes
- Wiki page for common error message for failed commands
- A utility to tell you how much memory the algorithm is occupying
- A utility to ensure that the algorithm has no memory leaks
- Fuzz test the maze loading utilities
- Clang compiler support
- Use tr() on all user-facing strings to make text translatable
- Show the timestamp that the algorithms were last built
- A button to randomly select a maze file
- Add stepper motor support in continuous mode
- Bluetooth communication with an actual robot
    - Would allow us to see where the robot thinks it is
- Monitor algo directories using FileSystemWatcher
    - Make it obvious when the user should re-build their algo
- Add sensor types (digital vs. analog)
- Look into using vsync so the graphics don't tear
- Make it possible to change the goal location (for smaller mazes)
- Fix existing maze file formats

Cleanup
=======
- Update the wiki
- Fix File Dialog perf (it's the QLabel perf)
- Don't allow continuous algorithms to be paused
- Remove continuous algorithm support from the interfaces, for now
- Prevent m_mouse segfaults
    - Audit all code that's touched by multiple threads (i.e., everything in
      Mouse, Model, etc.)
    - Remove ContainerUtilities.h, which copy containers unnecessarily (and
      cause implicit sharing segfaults)
- Don't actually log within mouse interface - surface errors a different way
- Improve sim time accuracy (time-to-center seems to be a function of sim speed)
- Improve CPU utilization (it's too high)
- Call all "algo stop" functions during Window initialization
    - Deduplicate initialization with "stop algo" code (and "failed algo" code)
- Make a note about why we write NO_ACK_STRING for some dispatched commands
    - It's a speed optimization, similar to UDP vs. TCP
- Kill the Layout class
- Kill the SimUtilities class, along with other platform dependent code
- Convert QVector<QString> to QStringList
- Replace push_back() with append()
- Audit codebase for unnecessary headers
- Lint the codebase
- Update the codebase to make use of implicit sharing
    - http://doc.qt.io/qt-5/implicit-sharing.html#implicitly-shared
- Get rid of unnecessary QString wrapping, like QString(<SOME-QSTRING>)
- Deduplicate some import/edit/validate logic between mouse and maze algo
- Use keyword explicit on one argument constructors
- Ensure UI widget spacing is sane
- Use const auto& for for-loops where possible

Unsorted
========
- A utility to tell you whether or not the stdlib is used
- Make out-of-bounds position not crash the simulator
- MacOS retina https://github.com/vispy/vispy/issues/99
- Button for toggling the maze views
- Verify that the tile text automatically refresh when we set tileGraphicTextMaxSize
- Maze loading
    - Use extension to determine which method to call
- Clean up mouseInterface/controller and MainWindow/controller (MouseAlgoUtilities)
- Add controls about UI
- Ensure all imports are correct
- Fix collision detection
- Use default initialization of unit types where possible
    - Rather than explicity calling a constructor with a "0" arg
- Replace "import algorithm" with "new algorithm"
- Rename the whole texture vs polygon thing
- Set up tests/build framework
- Make all classes QObjects
- QSet instead of QVector in some places
- char to QChar
- Replace QPair with structs
- Logging is too expensive
    - Causing freezing and crashing
    - Generalize a method for counting and logging few times, 1 per second
    - Write some code to make sure update throughput is good
        - 99% of updates are 1ms apart, or something like that
    - Check the sleep duration of updates to make sure that we're actually doing 1000 updates per second
- Draw dots on edges
- Make it more obvious which widgets are read-only
    - Use QLabel with border
- Discrete mode speed...
    - Ensure that the wheel speed is not too high in the MouseInterface (so that the mouse does not clip through walls)
    - In discrete mode, the mouse can overturn or go too far if the sim speed
      is to high. We need to implement some logic that checks this ahead of
      time and then just teleports... sort of like bullets in other physics
      simulators (note that the elapsed sim time should only be updated as much
      as necessary to get the mouse to its destination)
- Make an algo that can switch between tile edge movements and not
    - Proof of concept
- Add a way to time the algorithms
    - Make sure consistent at all speeds
- Add debugging tips
    - Note about what to do if the simulator says that it's an invalid algo (how to debug)
- Make some video tutorials
- Make a mechanism for easily merging the a discrete algo into a continuous one
    - Figure out how to bring the high level logic into the control algo
- Make MouseInterface into an interface, and then implement the simulator interface, make a real-world interface
- Figure out how to get rid of ifdefs in algorithm
- Make a SimInterface and make sure it's easy to "extract" an algorithm for use on Arduino
- Figure out a nice way to use a vector for tiles for the simulation, but an array for real life
- Sensor readings for walls other than front, left right (i.e., two cells ahead of you)
- Include Arduino "types" with the MouseInterface
- More Arduino function support
- Add gridlines (see https://www.youtube.com/watch?v=LGRutv9cGBA and https://www.youtube.com/watch?v=gtnTBf5ItEc)
    - Variable width?
    - Add trajectory lines (see https://www.youtube.com/watch?v=kgJClVCPu3w)
- Buffer communication (like with the real robot)
- Continuous performance
    - Fix/check continuous algo
    - CPU with megaMouse.xml
    - MinSleepDuration is a little bit weird - sometimes we try to sleep less than that
- Downward sensor
- Kill toLatin1() calls
- Add colors to stats/outputs...
    - Makes it easier to know that it's not a button
- Arduino int could be 16 bits
- Pass by reference or value in lambdas
- Rule of thumb - if it has state, make it a singleton instead of a static class
    - Rewrite singleton access patterns to be a little more readable (kill S(), P(), etc.)
    - All singletons should have an init function
    - Param and State still need to be converted
- Make a system for nearly instantly checking stats on many mazes:
    - like unit tests
    - solved or not
    - how many steps
    - ave steps
    - etc.
- Debugging tips
- Sensor read duration investigation
- Locking for sensor reads?
- How is wheelAdjustmentFactor working for default.xml?
    - Why do we have to use both contributions in both iterations?
- Change float to units (including in the ParamParser and Param class)
- Type safety (in terms of units) of the parameters
- Can we use mutable in the MouseInterface so declare wall on read is const? Make other methods in that class const?
- Make methods in IMouseInterface such as millis(), so that they can be called without "m_mouse->"
- Rename min-sleep-duration to something better
- Disable some copy constructors (wheels, sensors, etc.)
- Return const references, use const references in loops and especially in map iterations..., etc.
- Make a class for commonly used measurements
    - tileWidth
    - halfWallWidth
- If the wheel is at the center, it should have zero turn contribution, not infinite
- Organize params in res/parameters.xml and Param.h/.cpp
- Improve sensor readings by getting the actual complete polygon, not approximated
- Break up MouseInterface into DiscreteMouseInterface and ContinuousMouseInterface
- isDiscreteInterfaceCompatible and isContinuousInterfaceCompatible
- Set acceleration of all moving/rotating things
- Make some default Arduino implementations for some functions
- Testing for resource existence (like the shaders, font images, etc.)
- Xorg and compiz performance...
- Shortest path graphic (display the shortest path)
- Write a good continuous algorithm
- Interrupt interface
- Run the program with maximum thread priority
- Crash recovery semantics
    - Set a flag, let the algo check the flag
- Draw a path of where the robot has been (the history object could be used for recording as well)
- Write scripts we can execute (like unit tests) to make sure that our docs are in sync with the code
- Replace the convexHull with union in the mouse collision polygon initialization
- XML schema/validators
- Make diagonals smoother
    - make an option for smooth edge movements, including regular turns
    - https://www.youtube.com/watch?v=HPvke3fknrc
    - Stringing together diagonal movements (with smooth turns) is difficult
    - Use small curves to do diagonals
- Put MazeView into MazeGraphic (it should all be encapsulated in there)
- Update the UI if the QSettings file is changed manually
- Add gear ratio to the wheels
- Current rotation jumps between 0 and 360 on megaMouse right wall follow
- Pointers to wall polygons in each tile, save space by not breaking up walls/corners into multiple tiles
- Get rid of as much platform dependant code as possible
- Clean up / improve the coding standards
- Optimize for space in the TEXTURE_CPU_BUFFER and GRAPHIC_CPU_BUFFER - Don't repeat vertices
- Update the check_params.py script to make sure that the return type of the
  Param methods is the same as the type of the member variable
- Add a way to change the mouse color
- Crash animation for both modes
- Unit tests
- Try to break the simulator from within the algos, make it tamper-resistant
- Enable steering the wheels and sensors during run
- Implicit shared of QString, pass by value
- Javadoc as much as possible
- Make sure all QObjects have parents
- Use uniform syntax for Signal/slot connections
- Rename TriangleTexture and VertexTexture
- Rename Tile to Cell
- Write some unittests (QT Unit test)
- Run the parameter consistency script, make sure values align
- Convert primitive types to GL types (or vice versa)
- Return const references instead of values (pointers???)
- Reduce includes as much as possible
- Change "bool foo(false)" to "bool foo = false" for primitive - they look like function calls
- write "isDirectory(std::string path)" and enforce this where necessary
- write getRandom() and replace old calls to rand()
- Performance on different systems???
- The mouse sometimes rotates weirdly (goes slowly, backwards for a second, and
  then forwards fast. You can see what I mean by just increasing the view
  length of the sensors for a continuous algo)
- Pass things by reference when possible
- Give better explanations for the GeometryUtilities
- Buffer the declared walls and include a quick "resetWalls()" method
- Put consts in GraphicUtilties (and other classes)
- Replace braket notation with methods (walls[NORTH] = true -> walls.insert(std::make_pair(NORTH, true)));
- Document the confusing coordinate systems
- for each loop iterations should use const references
- Upgrade convert_mazes.py to Python 3
- P() be const? S()?
- init vs initialize
- Rename Tile to Cell?
- Fix up so that -Wall -Werror -pedantic-error flags pass
- -Wall: Show all warning messages
- -Werror: Fail compilation on warnings
- -pedantic-errors: Flag even the most pedantic of errors
- You can add layouts to layouts
    - make sure I'm not creating any unnecessary widgets
- Use the "widget" suffix for all user-defined widgets

// TODO: MACK
// +++
// 
// +++
// Crashes the simulator

Lessons
=======
- Map::values and Map::keys copy elements - use iterators to avoid this
- Map::value creates copies - you can use Map::operator[] to avoid this
