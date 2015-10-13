# mouse

This directory contains all code relevant to Micromouse algorithms. Some things
to note:

* `IMouseAlgorithm.h` is the "interface" that every Micromouse algorithm should
implement.
* You'll need to add any new algorithm to the `MouseAlgorithms.cpp` file before
it'll be accessible within `res/parameters.xml`.
* For an example, see the `rightWallFollow/` directory.
