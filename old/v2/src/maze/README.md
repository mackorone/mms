# maze

This directory contains all code relevant to user-created maze-generation
algorithms. Some things to note:

* `IMazeAlgorithm.h` is the "interface" that every maze-generation algorithm
should implement.
* You'll need to add any new maze-generation algorithm to the
`MazeAlgorithms.cpp` file before it'll be accessible within
`res/parameters.xml`.
* For an example, see the `randomize/` directory.
