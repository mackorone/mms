# src

This directory contains all of the source code for *mms*. That is, all compiled
C++ code lives within this directory. This makes it easy to, within the
Makefile (which lives in the project root directory), generate a list of build
rules - we can simply search this directory for files that end in `.cpp`.

The subdirectories are used as follows:

* `lib` - Contains code (mostly) borrowed from other open source projects
* `maze` - Contains code for all maze generation algorithms
* `mouse` - Contains code for all mouse (maze-solving) algorithms
* `sim` - Contains code internal to the simulator
