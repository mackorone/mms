# mms

![](https://github.com/mackorone/mms/wiki/imgs/mms.png)
![](https://github.com/mackorone/mms/wiki/imgs/mms.gif)

## About

*mms* is a Micromouse simulator. It allows programmers to write and test
Micromouse code as if it were running on an actual robot - and so much more.

It has two modes of operation, `DISCRETE` and `CONTINUOUS`, which offer
different levels of abstraction for writing maze-solving and robot-control
code, respectively. This means that programmer *X* can write code that gets the
robot from point *A* to point *B* while programmer *Y* ensures you don't crash
along the way. And all of *that* can happen while engineer *Z* tries to figure
out how to properly power a stepper motor.

## Features

* Provides a framework for:
    * Writing portable maze-solving code
    * Writing portable robot-control code
    * Writing maze-generation code, should you be so inclined
* Provides mechanisms for displaying what your robot knows as it explores the maze, including:
    * Setting the color of the tiles
    * Declaring the existence of walls
    * Keeping track of where you robot has been
    * Drawing text on the tiles
* Supports loading mazes from `.maz` files
* Highly configurable and customizable

## Installation, Building, and Running

#### Ubuntu

First, install the necessary software packages:
```bash
sudo apt-get install g++ freeglut3-dev libglew-dev
```
Then build the project:
```bash
make
```
Now run it:
```bash
./bin/mms
```

#### Other Platforms

[Windows](https://www.github.com/mackorone/mms/wiki/Windows\ Installation)

## Writing An Algorithm

#### Step 1: Create a directory for your algorithm:

```
mkdir src/mouse/foo
```

#### Step 2: Define a class that implements the IMouseAlgorithm interface:

**`src/mouse/foo/Bar.h`**
```c++
#pragma once

#include "../IMouseAlgorithm.h"

namespace foo {

class Bar : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace foo
```

**`src/mouse/foo/Bar.cpp`**
```c++
#include "Bar.h"

namespace foo {

void Bar::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

    // The mouse will move forward one tile and then stop
    mouse->moveForward();
}

} // namespace foo
```

#### Step 3: Add your class to the list of algorithms:

**`src/mouse/MouseAlgorithms.cpp`**
```c++
#include "MouseAlgorithms.h"
...
#include "foo/Bar.h"
...
std::pair<bool, IMouseAlgorithm*> MouseAlgorithms::helper(...) {
    ...
    ALGO("myFooBar", new foo::Bar());
    ...
}
```


#### Step 4: Update the `mouse-algorithm` parameter:

**`res/parameters.xml`**
```xml
...
<!-- Mouse Parameters -->
<mouse-algorithm>myFooBar</mouse-algorithm>
...
```

## Authors

| Name              | Email                | Role                  |
|-------------------|----------------------|-----------------------|
| Mack Ward         | mward4@buffalo.edu   | Author and Maintainer |
| Tomasz Pietruszka | tomaszpi@buffalo.edu | Contributor           |
| Mitchell Rathbun  | mrathbun@buffalo.edu | Contributor           |
| Sam Siegart       | srsiegar@buffalo.edu | Contributor           |
| Kyle Thompson     | kt49@buffalo.edu     | Contributor           |

## Contributing Projects

| Name                                                          | Author            | Use Case              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [easyloggingpp](https://github.com/easylogging/easyloggingpp) | muflihun.com      | Logging               |
| [fontstash](https://github.com/akrinke/Font-Stash)            | Andreas Krinke    | Text Rendering        |
| [opengl-series](https://github.com/tomdalling/opengl-series)  | Thomas Dalling    | OpenGL Functionality  |
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [pugixml](https://github.com/zeux/pugixml)                    | Arseny Kapoulkine | XML Parsing           |
| [stb](https://github.com/nothings/stb)                        | Sean Barrett      | Images and Fonts      |

## Wiki

See the [wiki](https://www.github.com/mackorone/mms/wiki) for more information and documentation.

Note that the *wiki* repository is contained within the *mms* repository as a
[submodule](https://git-scm.com/docs/git-submodule). This means that if you
want to see/edit the wiki source, you'll have to first clone (not download the
zip of) *mms*, and then initialize (and update) the submodule:

```bash
git clone https://github.com/mackorone/mms.git
cd mms/wiki
git submodule init
git submodule update
```

## Facebook

See the [Facebook page](https://www.facebook.com/mackorone.mms) for pictures,
video demos, and other media related to the simulator.
