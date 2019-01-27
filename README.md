# mms

![](https://github.com/mackorone/mms/blob/master/img/mms.gif)

## Table of Contents

1. [Introduction](https://github.com/mackorone/mms#introduction)
1. [Installation](https://github.com/mackorone/mms#installation)

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

#### `wallLeft`
* **Args:** None
* **Action:** None
* **Response:** `true` if there is a wall to the left of the robot, else `false`

#### `moveForward`
* **Args:** None
* **Action:** Move the robot forward by one cell
* **Response:**
  * `crash` if there is a wall in front of the robot
  * else `ack` once the movement completes

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
  * `C` - The character of the desired color
* **Action:** Set the color of the cell at the given position
* **Response:** None

The available colors are as follows:

| Char | Color       |
|------|-------------|
|  k   | Black       | 
|  b   | Blue        | 
|  a   | Gray        | 
|  c   | Cyan        | 
|  g   | Green       | 
|  o   | Orange      | 
|  r   | Red         | 
|  w   | White       | 
|  y   | Yellow      | 
|  B   | Dark Blue   | 
|  C   | Dark Cyan   | 
|  A   | Dark Gray   | 
|  G   | Dark Green  | 
|  R   | Dark Red    | 
|  Y   | Dark Yellow | 

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
  * `TEXT` - The desired text, max length 10
* **Action:** Set the text of the cell at the given position
* **Response:** None

The available characters are as follows:

|   |   |   |   |   |   |   |   |   |   |   |   |      |   |   |   |
|---|---|---|---|---|---|---|---|---|---|---|---|------|---|---|---|
|   | ! | " | # | $ | % | & | ' | ( | ) | * | + |  ,   | - | . | / |
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | : | ; |  <   | = | > | ? |
| @ | A | B | C | D | E | F | G | H | I | J | K |  L   | M | N | O |
| P | Q | R | S | T | U | V | W | X | Y | Z | [ |  \   | ] | ^ | _ |
| ` | a | b | c | d | e | f | g | h | i | j | k |  l   | m | n | o |
| p | q | r | s | t | u | v | w | x | y | z | { |&#124;| } | ~ |   |

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

## Maze Files

Format:

    X Y N E S W

Example:

    0 0 0 1 1 1
    0 1 1 0 0 1
    1 0 0 0 1 1
    1 1 1 1 0 0
    2 0 0 1 1 0
    2 1 1 1 0 1

Result:

    +---+---+---+
    |       |   |
    +   +   +   +
    |   |       |
    +---+---+---+

Requirements for use with simulator:

* Nonempty
* Rectangular
* Fully enclosed
* Internally consistent

Additional requirements for official Micromouse mazes:

* No inaccessible locations
* Exactly three starting walls
* Only one entrance to the center
* Has a hollow center, i.e., the center peg has no walls attached to it
* Has walls attached to every peg except the center peg
* Is unsolvable by a wall-following robot

Note: other formats are not supported, however there is a CLI tool that can
easily convert between formats: https://github.com/mackorone/maze

## Previous Versions

Previous versions of mms exist in the `old/` directory. Feel free to take a
look, but only the most recent version is supported.


## Dependencies

| Name                                                          | Author            | Used For              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [Qt](https://www.qt.io/)                                      | The Qt Company    | Framework and GUI     |
