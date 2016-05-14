Tile color is a mechanism for conveying arbitrary algorithm information to the
user or programmer. Here's the API for setting tile color:

```c++
// Sets the color of the tile at location (x, y) to the given color
void setTileColor(int x, int y, char color);

// Clears the color of the tile at location (x, y), i.e., sets the color of the
// tile back to the normal base color (usually black)
void clearTileColor(int x, int y);

// Clears the color from all tiles in the maze, in a more efficient way than
// just calling clearTileColor on all tiles
void clearAllTileColor();
```

### Available Colors

The available colors are defined in `src/sim/Color.h`. They are as follows:

| char | Color       |
|------|-------------|
|  k   | BLACK       | 
|  b   | BLUE        | 
|  a   | GRAY        | 
|  c   | CYAN        | 
|  g   | GREEN       | 
|  o   | ORANGE      | 
|  r   | RED         | 
|  w   | WHITE       | 
|  y   | YELLOW      | 
|  B   | DARK_BLUE   | 
|  C   | DARK_CYAN   | 
|  A   | DARK_GRAY   | 
|  G   | DARK_GREEN  | 
|  R   | DARK_RED    | 
|  Y   | DARK_YELLOW | 

### Example

A good example of how to use tile colors effectively is `MackAlgo`. The
algorithm uses colors to show the intended path of the mouse, which gets
adjusted as it gathers information about walls in the maze. The full intended
path is colored blue, and the tiles on the path where it knows that the path is
clear are colored yellow. Additionally, the center/destination is colored
green. Check it out and you'll see what I mean.

### Toggling Tile Colors

Tile colors may be on or off by default, based on the simulator
[Parameters](Parameters). As mentioned in [Keys](Keys), you can toggle the
colors on and off by pressing "c" while the simulation is running.

### `setTileBaseColorWhenDistanceDeclaredCorrectly()`

Just in case you may have missed it, there's a [Mouse Algorithm Option](Mouse
Algorithm Option) that allows the tile base color to be set automatically
whenever a tile distance is declared correctly. That is, if you return `true`
from `setTileBaseColorWhenDistanceDeclaredCorrectly()`, then whenever you call
`declareTileDistance()` and the distance that you provide for that tile is
correct, then the color of the tile will change automatically. This feature is
useful because it allows users to visualize when tile distances are declared
correctly. Note that whenever tile color is set, it just overwrites the
previous tile color. So if you're using this feature, you're probably not going
to want to use any of the tile color API methods, lest you overwrite (or clear)
some tile's color accidentally. 

For an example of an algorithm that uses this feature, see `FloodFill`.
