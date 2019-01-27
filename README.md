# mms

![](https://github.com/mackorone/mms/blob/master/img/mms.gif)

## Table of Contents

TODO

## Introduction

*mms* is a Micromouse simulator.

It makes it easy to write and test maze-solving code without a physical robot.

With it, you can:

* Test how your robot would behave in a real maze
* Visualize what your robot is thinking
  * Show known/unknown walls
  * Set the color of the cells
  * Display ASCII text on the cells
* Test your algorithm on custom maze files
* Simulate a crash-and-reset scenario
* Write code in any language you want

For information about Micromouse, see the [Micromouse Wikipedia page](http://en.wikipedia.org/wiki/Micromouse).

## Installation

#### Pre-compiled Binaries

In the near future, we'll provide pre-compiled binaries on these three platforms:

* Windows
* Mac OS X
* Linux 

Until then, you'll have to build from source.

#### Build From Source

1. Download Qt (see [this download page](https://www.qt.io/download/))
1. Use the `sim.pro` file in `src/sim` to configure and build the project
1. Run the resultant binary

For example, on Ubuntu, run the following:

```bash
cd src/sim
qmake
make
../../bin/sim
```

## Writing An Algorithm

#### Step 1: Create a directory for your algorithm:

```bash
mkdir ~/Desktop/MyAlgo
```

#### Step 2: Copy "stub" files in any directory in `src/mouse/templates`:

```bash
cp src/mouse/templates/c++/* ~/Desktop/MyAlgo
```

#### Step 3: Add some algorithm logic:

```bash
#include "Algo.h"

void Algo::solve(Interface* interface) {
    // TODO: implement the algorithm here
    if (interface->wallRight()) {
        interface->moveForward()
    }
    // ...
}
```

#### Step 4: Configure your algorithm:

Here's an example:

![](https://github.com/mackorone/mms/wiki/images/edit.png)


##  Button

## Reset Button

## Mouse API

## Cell Walls

## Cell Color

## Cell Text

## Reset Button

## Maze Format

    // Format:
    //
    //     X Y N E S W
    //
    // Example:
    //
    //     0 0 0 1 1 1
    //     0 1 1 0 0 1
    //     1 0 0 0 1 1
    //     1 1 1 1 0 0
    //     2 0 0 1 1 0
    //     2 1 1 1 0 1
    //
    // Result:
    //
    //     +---+---+---+
    //     |       |   |
    //     +   +   +   +
    //     |   |       |
    //     +---+---+---+


    * Loading mazes in many different formats (`.MAZ`, `.MZ2`, `.num`, `.map`)

#### Validity checks

### The `.maz` format

1. Each `.maz` file consists of `MAZE_WIDTH` by `MAZE_HEIGHT` newline-separated lines
2. Each line consists of 6 whitespace (preferably single-space) separated tokens
3. The first 2 tokens represent the `x` and `y` position of the tile, respectively
4. The last 4 tokens represent the existence of `north`, `east`, `south`, and
   `west` walls, respectively; `1` indicates the presence of wall and `0`
   indicates the absence of a wall
5. The lines should be sorted in order of their `x` position, and then
   subsorted in order of their `y` position

### Notes

1. The presence/absence of walls must be consistent between rows. That is, if a
   wall is present to the north of a particular tile, it should also be present to
   the south of the tile directly above the first tile.
2. In order for a maze file to be valid, the maze must be bounded on all sides.

### Example

A valid 3x2 maze file might look like:

```
0 0 0 1 1 1
0 1 1 0 0 1
1 0 0 0 1 1
1 1 1 0 0 0
2 0 1 1 1 0
2 1 1 1 1 0
```

And the maze corresponding to the file would look like:

```
|---------------------------------------------|
|                                             |
|                                             |
|                                             |
|     (0,1)           (1,1)         (2,1)     |
|                                             |
|                                             |
|                              ---------------|
|               |                             |
|               |                             |
|               |                             |
|     (0,0)     |     (1,0)         (2,0)     |
|               |                             |
|               |                             |
|---------------|-----------------------------|
```
## Older Versions

Previous incarnations of mms exist in the `old/` directory. Feel free to take a
look, but previous versions are currently unsupported.

## Dependencies

| Name                                                          | Author            | Used For              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [Qt](https://www.qt.io/)                                      | The Qt Company    | Framework and GUI     |
