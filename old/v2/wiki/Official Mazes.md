There are three types of mazes that the simulator deals with:

- Invalid and not official
- Valid but not official
- Valid and official

### Invalid Mazes

Invalid mazes are mazes that don't have all of the properties of valid mazes
(as listed below). These mazes can't be loaded by the simulator. If an invalid
maze is provided via a maze file or a maze generation algorithm, the simulator
will print an error message and exit. Simple as that.

### Valid, But Not Official, Mazes

Valid mazes have the following properties:
- Nonempty, i.e., at least one tile
- Rectangular
- Enclosed by walls on all sides
- Consistent walls, e.g., if tile (0,1) has a north wall, then tile (0,2)
  should have a south wall, etc.

However, if a maze is not official, it means that it doesn't have all of the
properties of official mazes (as listed below). If a valid but not official
maze is provided via a maze file or a maze generation algorithm, the simulator
will simply print a warning and simulate as normal.

### Official, And Therefore Valid, Mazes

Official mazes have all of the properties of valid mazes, and the following:

- No inaccessible locations
- Exactly three starting walls
- Only one entrance to the center
- Has a hollow center, i.e., the center peg has no walls attached to it
- Has walls attached to every peg except the center peg
- Is unsolvable by a wall-following robot

If a maze is official, it is necessarily valid. An official maze loads in the
simulator without anything printing out.

### Implementation

If you're interested in seeing how we classify a given maze, check out
`src/sim/MazeChecker.h` and `src/sim/MazeChecker.cpp`, which contain all of the
logic to perform the maze classification.

### `isOfficialMaze` Parameter

Just in case you missed it, one of the parameters to the `solve()` method,
which is defined for every `IMouseAlgorithm`, is `isOfficialMaze`. This
parameter is `true` is the maze is official, and `false` if the maze is valid
but not official. This information could allow an algorithm to perform certain
optimizations for official and unofficial maze.
