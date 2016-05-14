There are a few options that are available to mouse algorithms. They are
implemented as virtual methods in the `IMouseAlgorithm` class. That is, the
default options are defined in `src/mouse/IMouseAlgorithm.cpp`. If you wish to
use values other than the defaults, should override the virtual methods in you
algorithm (which should inherit from IMouseAlgorithm). For instance, suppose
your algorithm's `.h` and `.cpp` file looked like to following, respectively:

```c++
// LeftWallFollow.h

#pragma once

#include "../IMouseAlgorithm.h"

namespace leftWallFollow {

class LeftWallFollow : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

} // namespace leftWallFollow
```

```c++
// LeftWallFollow.cpp

#include "LeftWallFollow.h"

namespace leftWallFollow {

void LeftWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true) {
        leftWallFollowStep(mouse);
    }
}

void LeftWallFollow::leftWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}

} // namespace leftWallFollow
```

If you wanted to change the value of the `declareWallsOnRead` option, you would
override that method in `LeftWallFollow`, like:

```c++
// LeftWallFollow.h

#pragma once

#include "../IMouseAlgorithm.h"

namespace leftWallFollow {

class LeftWallFollow : public IMouseAlgorithm {

public:
    bool declareWallOnRead() const; // Note the "const"
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

private:
    void leftWallFollowStep(sim::MouseInterface* mouse);
};

} // namespace leftWallFollow
```

```c++
// LeftWallFollow.cpp

#include "LeftWallFollow.h"

namespace leftWallFollow {

bool LeftWallFollow::declareWallOnRead() const { // Note the "const"
    return true;
}

void LeftWallFollow::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {
    while (true) {
        leftWallFollowStep(mouse);
    }
}

void LeftWallFollow::leftWallFollowStep(sim::MouseInterface* mouse) {
    if (!mouse->wallLeft()){
        mouse->turnLeft();
    }
    while (mouse->wallFront()){
        mouse->turnRight();
    }
    mouse->moveForward();
}

} // namespace leftWallFollow
```

One important thing to remember is that all of the mouse algorithm options are
implemented as const methods. If you forget to put `const` after the method
name, the option you specified won't have any effect.

Right now, the algorithm options are evaluated at either the start of the
simulator, or throughout the simulation. That is, the static options are all
evaluated only once, and changing the return value of those functions won't do
anything. However, the dynamic options are evaluated continuously, which means
that you can do fancy things like change the return values of the functions
depending on the state of your algorithm. See the `LeftWallFollow` algorithm
for an example of how to change the value of these options at runtime.

Here are all of the options available to mouse algorithms:

### Static options for both interfaces

###### `virtual std::string mouseFile() const;`

The mouse file, relative to `res/mouse/`, which specifies the physical
properties of the mouse. For more info, see [Mouse Spec File](Mouse Spec File).

###### `virtual std::string interfaceType() const;`

The interface type of the mouse algorithm. Can be either `DISCRETE` or
`CONTINUOUS`. For more information, see [DISCRETE vs CONTINUOUS](DISCRETE vs
CONTINUOUS).

###### `virtual std::string initialDirection() const;`

The direction that the mouse should be facing at the start of executing. Can be one of:
- `NORTH` - facing north
- `EAST` - facing north
- `SOUTH` - facing north
- `WEST` - facing north
- `OPENING` - facing either north or east, whichever is the opening, if one exists
- `WALL` - facing either north or east, whichever is the wall, if one exists

###### `virtual int tileTextNumberOfRows() const;`

The number of rows of text per tile. It's a good idea to keep this as small as
possible, since it's expensive to draw lots of characters on every tile. See
[Tile Text](Tile Text) for more information.

###### `virtual int tileTextNumberOfCols() const;`

The number of columns of text per tile. It's a good idea to keep this as small
as possible, since it's expensive to draw lots of characters on every tile. See
[Tile Text](Tile Text) for more information.

### Dynamic options for both interface types

###### `virtual bool allowOmniscience() const;`

Whether or not to allow the algorithm to call the omniscience methods, which
give the algorithm access to information it wouldn't normally have, including
- Current mouse position
- Current mouse rotation

###### `virtual bool automaticallyClearFog() const;`

Whether or not the simulator should automatically clear tile fog when the mouse
enters a new tile. See [Tile Fog](Tile Fog) for more information.

###### `virtual bool declareBothWallHalves() const;`

Whether or not declaring one side of a wall should cause the other side of the
wall to also be declared. For example, when this is true, declaring the north
wall of tile (0,1) would also cause the south wall of tile (0,2) to be declared
similarly. The reason that this isn't the default behavior is because most
algorithms store their wall information on a tile by tile basis. By forcing the
algorithm to explicitly declare both walls, we increase the odds that the
internal representation of the walls stays consistent. See [Tile Walls](Tile
Walls) for more information.

###### `virtual bool setTileTextWhenDistanceDeclared() const;`

Whether or not the tile text should be set to the declared distance, if and
when a tile distance is declared. See [Tile Text](Tile Text) and [Tile
Distance](Tile Distance) for more information.

###### `virtual bool setTileBaseColorWhenDistanceDeclaredCorrectly() const;`

Whether or not the tile base color should be set to something other than the
default tile base color, if and when the distance is declared correctly. This
option is used to give visual feedback about when the tile distances are known
to the mouse algorithm. For more information, see [Tile Color](Tile Color) and
[Tile Distance](Tile Distance).

### Static options for the DISCRETE interface

###### `virtual double wheelSpeedFraction() const;`
The fraction of the maximum wheel speed that should be used for movement in
`DISCRETE` mode. For most mice, this can just be `1`. However, some mice, like
`megaMouse.xml`, have very high maximum speeds, that aren't suitable for
regular maze traversal. You can use this method to specify the fraction of the
maximum wheel speed that should be used. See [Mouse Spec File](Mouse Spec File)
for more information.

### Dynamic options for the DISCRETE interface

###### `virtual bool declareWallOnRead() const;`
Whether or not the walls of the maze should be declared whenever the algorithm
checks for the existence of the wall. Normally, tile wall declaration is used
to convey information to the programmer about the mouse's knowledge as it
explores and learns the maze. This option allows us to show to users that a
wall has been inspected, without necessarily having to keep track of exactly
which walls we're inspecting. For example, the wall following algorithms use
this to give visual information about where the mouse has been. See [Tile
Walls](Tile Walls) for more information.

###### `virtual bool useTileEdgeMovements() const;`
Whether or not the discrete movements should start and stop on tile edges, as
opposed to tile centers. If `true`, you must use only the special `DISCRETE`
methods that are outlined in [The Complete Mouse API](The Complete Mouse API).
If `false`, you must use only the basic `DISCRETE` methods outlined in the same
document.
