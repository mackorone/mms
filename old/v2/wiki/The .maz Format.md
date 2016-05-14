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
