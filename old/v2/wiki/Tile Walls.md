Tile walls are a mechanism for conveying the wall information of the mouse
algorithm. It's a way of visually displaying exactly where the mouse thinks
walls are, and where it thinks they are not. This is extremely valuable
information for the user/programmer to have, and it much more intuitive than
trying to print/display the wall information textually.

Tile walls are "declared" by the algorithm. That is, the algorithm is
responsible for declaring (to the simulator) that it thinks that there is a
wall in a particular location. The simulator can then change the color of the
wall place-holder according to whether or not the wall declaration was correct
or incorrect, giving instantaneous feedback to users about the proper or
improper functioning of the algorithm.

The API for declaring tile walls is as follows:

```c++
void declareWall(int x, int y, char direction, bool wallExists);
void undeclareWall(int x, int y, char direction);
```

The best way to use wall declarations effectively is to couple them together
with the logic that updates the algorithm's internal representation of the maze.
That is, in order for a mouse to find the center of the maze, it needs to keep
track of the walls. Most people store this information in an array or vector.
As the mouse explores the maze, it updates the information in the array/vector.
In order to ensure that the walls are declared when the internal wall
information is updated, it's wise to make a helper function that updates the
internal representation and declares walls. Then, instead of updating the
array/vector directly, you can just call the helper function. And then you can
be sure that the wall declaration visualization will always match your mouse's
internal representation of the maze.

For example:

```c++
// Suppose we have a 2D array of myTileStruct objects (which store wall info)
myTileStruct maze[16][16];

// Don't do this everywhere in the codebase - it's too difficult to remember to
// call declareWall each time you update maze
maze[0][0].wallNorth = true;
mouse->declareWall(0, 0, 'n', true);

// Instead, define a helper function like this
void setWall(int x, int y, char dir, bool isWall) {

    // Update the internal representation
    switch (dir) {
        case 'n': // north
            maze[0][0].wallNorth = isWall;
            break
        ...
    }

    // Declare the wall
    mouse->declareWall(x, y, dir, isWall);
}

// And then use it like this, so that you don't have to remember to call
// declareWall each and every time you update the internal state of the maze.
setWall(0, 0, 'n', true);
```

A good example of an algorithm that does this is `MackAlgo`. Check out
`src/mouse/mackAlgo/Cell.cpp` for some code that follows this pattern.

### Toggling Wall Truth

Walls truth may be on or off by default, based on the simulator
[Parameters](Parameters). As mentioned in [Keys](Keys), you can toggle between
seeing the wall truth and the algorithm's internal representation of the maze
by pressing "t" while the simulation is running.

### `declareWallsOnRead()`

Just in case you may have missed it, there's a [Mouse Algorithm Option](Mouse
Algorithm Options) that allows walls to be declared automatically whenever their
existence is queried. That is, if you return `true` from `declareWallsOnRead()`,
then whenever you call any of the `DISCRETE` API methods `wallFront()`,
`wallLeft()`, or `wallRight()`, the wall that you inspected will automatically
be declared. The use case for this is something like a wall-following robot,
where the wall declarations are used to convey information about where the
robot has been, and not necessarily the internal representation of the maze. In
general, it's best to declare the walls manually so that you can have more
confidence that the walls displayed correlate to the internal representation of
the maze.
