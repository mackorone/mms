# mms

![](https://raw.githubusercontent.com/wiki/mackorone/mms/logo.png)

## What

### About

*mms* is a Micromouse simulator. It allows programmers to write and test
Micromouse code as if it were running on an actual robot - and so much more.

It has two modes of operation, `DISCRETE` and `CONTINUOUS`, which offer
different layers of abstraction for writing maze-solving and robot-control
code, respectively. This means that programmer *X* can write code that gets the
robot from point *A* to point *B* while programmer *Y* ensures you don't crash
along the way. And all of *that* can happen while engineer *Z* tries to figure
out how to properly power a stepper motor.

### Features

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

## How

Writing an algorithm for *mms* is as easy as 1, 2, 3:

1. Create a directory within `src/mouse` for your algorithm:

```
mkdir src/mouse/foo
```

Then, withing that directory, define a class that implements the IMouseAlgorithm interface, like:

#### src/mouse/foo/Foo.h
```c++
#pragma once

#include "../IMouseAlgorithm.h"

namespace foo {

class Foo : public IMouseAlgorithm {

public:
    std::string mouseFile() const;
    std::string interfaceType() const;
    void solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse);

};

} // namespace foo
```

#### src/mouse/foo/Foo.cpp
```c++

namespace foo {

std::string Foo::mouseFile() const {
    return "mouse.xml";
}

std::string Foo::interfaceType() const {
    return "DISCRETE";
}

void Foo::solve(int mazeWidth, int mazeHeight, char initialDirection, sim::MouseInterface* mouse) {
    // The mouse will move forward one tile and then stop
    mouse->moveForward();
}

} // namespace foo
```

2. Add your class to the the list of algorithms in `src/mouse/MouseAlgorithms.cpp`:

#### src/mouse/MouseAlgorithms.cpp
```c++
#include "MouseAlgorithms.h"

...
#include "foo/Foo.h"
...

std::pair<bool, IMouseAlgorithm*> MouseAlgorithms::helper(const std::string& str, bool justChecking) {

    ...
    ALGO("Foo", new foo::Foo());
    ...
}
```


3. Change the mouse-algorithm parameter in `res/parameters.xml` to the name of your algorithm:

#### res/parameters.xml
```xml
...
<!-- Mouse Parameters -->
<mouse-algorithm>Foo</mouse-algorithm> <!-- The name as specified in src/mouse/MouseAlgorithms.cpp -->
...
```

## Who

### Authors

| Name              | Email                | Role                  |
|-------------------|----------------------|-----------------------|
| Mack Ward         | mward4@buffalo.edu   | Author and Maintainer |
| James Mazur       | jamesmaz@buffalo.edu | Contributor           |
| Tomasz Pietruszka | tomaszpi@buffalo.edu | Contributor           |
| Mitchell Rathbun  | mrathbun@buffalo.edu | Contributor           |
| Sam Siegart       | srsiegar@buffalo.edu | Contributor           |
| Kyle Thompson     | kt49@buffalo.edu     | Contributor           |

### Contributing Projects

| Name                                                          | Author            | Use Case              |
|---------------------------------------------------------------|-------------------|-----------------------|
| [easyloggingpp](https://github.com/easylogging/easyloggingpp) | muflihun.com      | Logging               |
| [fontstash](https://github.com/akrinke/Font-Stash)            | Andreas Krinke    | Text Rendering        |
| [opengl-series](https://github.com/tomdalling/opengl-series)  | Thomas Dalling    | OpenGL Functionality  |
| [polypartition](https://github.com/ivanfratric/polypartition) | Ivan Fratric      | Polygon Triangulation |
| [pugixml](https://github.com/zeux/pugixml)                    | Arseny Kapoulkine | XML Parsing           |
| [stb](https://github.com/nothings/stb)                        | Sean Barrett      | Images and Fonts      |

## Where

See the [mms wiki](https://www.github.com/mackorone/mms/wiki) for more information and documentation.
