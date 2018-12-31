# mms

![](https://github.com/mackorone/mms/wiki/images/mms.png)
![](https://github.com/mackorone/mms/wiki/images/mms.gif)

## About

mms is a Micromouse simulator.

It aims to solve two main problems:

1. It's difficult to write Micromouse code *without* a working robot
1. It's difficult to write Micromouse code *with* a working robot

To elaborate:

1. Without running it on *something* (normally a physical robot), there's no
way to ensure that the maze-solving and obstacle-avoidance code works as
expected
1. Even *with* a working robot, re-uploading and re-running the code is tedious
and error-prone; it's difficult to reproduce and debug anomalies when they occur

mms solves these problems by:

1. Providing a framework for writing, and an environment for testing, both
maze-solving and obstacle-avoidance code
1. Making it easy to run, re-rerun, and understand your code, as it's executing
(via special logging and graphical utilities)

## Features

* Provides a framework for:
    * Writing maze-generation code
    * Writing and testing maze-solving code
    * Writing and testing obstacle-avoidance code

* Provides mechanisms for understanding your code, as it executes:
    * Drawing text on the cells
    * Setting the color of the cells
    * Declaring the existence of walls
    * Showing where your robot has been

* Supports:
    * Loading mazes in many different formats (`.MAZ`, `.MZ2`, `.num`, `.map`)
    * Simulating the behavior of many types of robots, specified via mouse files
    * Algorithms written in many different languages (currently C/C++, Java, and Python)

## Installation, Building, and Running

#### Pre-compiled Binaries

In the near future, we'll provide pre-compiled binaries on these three platforms:

* Windows
* Mac OS X
* Linux 

Until then, you'll have to build from source.

#### Building From Source

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

## Wiki

See the [wiki](https://www.github.com/mackorone/mms/wiki) for more information and documentation.

Note that the wiki repository is contained within the mms repository as a
[submodule](https://git-scm.com/docs/git-submodule). This means that if you
want to see/edit the wiki source, you'll have to first clone (not download the
zip of) mms, and then initialize (and update) the submodule:

```bash
git clone https://github.com/mackorone/mms.git
cd mms/wiki
git submodule init
git submodule update
```

## Facebook

See the [Facebook page](https://www.facebook.com/mackorone.mms) for pictures,
video demos, and other media related to the simulator.

## Older Versions

Previous incarnations of mms exist in the `old` directory. Feel free to take a
peek, but previous versions will (obviously) not receive the same attention and
support as the current version.

## Contributing Projects

| Name                                                          | Author            | Use Case              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [Qt](https://www.qt.io/)                                      | The Qt Company    | Framework and GUI     |

## Authors

| Name              | Email                | Role                  |
|-------------------|----------------------|-----------------------|
| Mack Ward         | mward4@buffalo.edu   | Author and Maintainer |
| Tomasz Pietruszka | tomaszpi@buffalo.edu | Contributor           |
| Mitchell Rathbun  | mrathbun@buffalo.edu | Contributor           |
| Sam Siegart       | srsiegar@buffalo.edu | Contributor           |
| Kyle Thompson     | kt49@buffalo.edu     | Contributor           |
