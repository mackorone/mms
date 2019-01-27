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

## Quick Start

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


## Configuration

## How It Works

## Mouse API

Algorithms communicate with the simulator via stdin/stdout. To issue a command,
simply print to stdout. To read a response, simply read from stdin. All valid
commands are listed below. Invalid commands are simply ignored.

For commands that return a response, it's recommended to wait for the response
before issuing additional commands.

#### Summary 

```c++
int mazeWidth();
int mazeHeight();

bool wallFront();
bool wallRight();
bool wallLeft();

void moveForward();
void turnRight();
void turnLeft();

void setWall(int x, int y, char direction);
void clearWall(int x, int y, char direction);

void setColor(int x, int y, char color);
void clearColor(int x, int y);
void clearAllColor();

void setText(int x, int y, std::string text);
void clearText(int x, int y);
void clearAllText();

bool wasReset();
void ackReset();
```

#### `mazeWidth`
* **Args:** None
* **Action:** None
* **Response:** The height of the maze

#### `mazeHeight`
* **Args:** None
* **Action:** None
* **Response:** The width of the maze

#### `wallFront`
* **Args:** None
* **Action:** None
* **Response:** `true` if there is a wall in front of the robot, else `false`

#### `wallRight`
* **Args:** None
* **Action:** None
* **Response:** `true` if there is a wall to the right of the robot, else `false`

#### `wallRight`
* **Args:** None
* **Action:** None
* **Response:** `true` if there is a wall to the left of the robot, else `false`

#### `moveForward`
* **Args:** None
* **Action:** Move the robot forward by one cell
* **Response:**
  * `crash` if there is a wall in front of the robot
  * `ack` once the movement completes

#### `turnRight`
* **Args:** None
* **Action:** Turn the robot ninty degrees to the right
* **Response:** `ack` once the movement completes

#### `turnLeft`
* **Args:** None
* **Action:** Turn the robot ninty degrees to the left
* **Response:** `ack` once the movement completes

#### `setWall X Y D`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
  * `D` - The direction of the wall: `n`, `e`, `s`, or `w`
* **Action:** Display a wall at the given position
* **Response:** None

#### `clearWall X Y D`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
  * `D` - The direction of the wall: `n`, `e`, `s`, or `w`
* **Action:** Clear the wall at the given position
* **Response:** None

#### `setColor X Y C`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
  * `C` - The character of the desired [color](https://github.com/mackorone/mms#cell-color)
* **Action:** Set the color of the cell at the given position
* **Response:** None

#### `clearColor X Y`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
* **Action:** Clear the color of the cell at the given position
* **Response:** None

#### `clearAllColor`
* **Args:** None
* **Action:** Clear the color of all cells
* **Response:** None

#### `setText X Y TEXT`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
  * `TEXT` - The desired [text](https://github.com/mackorone/mms#cell-text), max length 10
* **Action:** Set the text of the cell at the given position
* **Response:** None

#### `clearText X Y`
* **Args:**
  * `X` - The X coordinate of the cell
  * `Y` - The Y coordinate of the cell
* **Action:** Clear the text of the cell at the given position
* **Response:** None

#### `clearAllColor`
* **Args:** None
* **Action:** Clear the text of all cells
* **Response:** None

#### `wasReset`
* **Args:** None
* **Action:** None
* **Response:** `true` if the reset button was pressed, else `false`

#### `ackReset`
* **Args:** None
* **Action:** Allow the mouse to be moved back to the start of the maze
* **Response:** `ack` once the movement completes


## Cell Walls

## Cell Color

## Cell Text

## Reset Button

## Speed Controls

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

## Previous Versions

Previous versions of mms exist in the `old/` directory. Feel free to take a
look, but only the most recent version is supported.


## Dependencies

| Name                                                          | Author            | Used For              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [Qt](https://www.qt.io/)                                      | The Qt Company    | Framework and GUI     |
