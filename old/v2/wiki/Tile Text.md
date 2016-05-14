Tile text is yet another mechanism for conveying algorithm information to the user.
Here's the API for setting tile text:

```c++
// Sets the text of the tile at location (x, y) to the given text
void setTileText(int x, int y, const std::string& text);

// Clears the text of the tile at location (x, y), equivalnet to calling
// setTileText() and passing in the empty string
void clearTileText(int x, int y);

// Clears the text from all tiles in the maze, in a more efficient way than
// just calling clearTileText on all tiles
void clearAllTileText();
```

### Allowable Characters

Only certain characters can be used in tile text. They are as follows:

| A | V | A | I | L | A | B | L | E |   | C | H |  A   | R | S | : |
|---|---|---|---|---|---|---|---|---|---|---|---|------|---|---|---|
|   | ! | " | # | $ | % | & | ' | ( | ) | * | + |  ,   | - | . | / |
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | : | ; |  <   | = | > | ? |
| @ | A | B | C | D | E | F | G | H | I | J | K |  L   | M | N | O |
| P | Q | R | S | T | U | V | W | X | Y | Z | [ |  \   | ] | ^ | _ |
| ` | a | b | c | d | e | f | g | h | i | j | k |  l   | m | n | o |
| p | q | r | s | t | u | v | w | x | y | z | { |&#124;| } | ~ |   |

Note that the first row in the table above is just the title of the table. Also
note that the space character, a valid character to use in tile text, is both
the first and last character in the table (I had one extra slot in the table).

Any invalid characters, such as a newline or tab, will be replaced with
`default-tile-text-character`, as described in [Parameters](Parameters),
by default a "?" character.

### Tile Text Visibility

Tile text visibilty can be toggled by pressing the "x" [key](Keys) while the
simulator is running.

The `default-tile-text-visible` [parameter](Parameters) controls whether or not
the text is visible by default, i.e., at the start of the program.

### Alignment

The `tile-text-alignment` [parameter](Parameters), controls the alignment of
the tile text. The nine possible alignment values are:

| POSSIBLE    | ALIGNMENT     | VALUES       |
|-------------|---------------|--------------|
|`UPPER_LEFT` |`UPPER_CENTER` |`UPPER_RIGHT` |
|`CENTER_LEFT`|`CENTER_CENTER`|`CENTER_RIGHT`|
|`LOWER_LEFT` |`LOWER_CENTER` |`LOWER_RIGHT` |

Note that the first row of the table is just the table title. Also note that
the default value is `CENTER_CENTER`. And just for the record, this is arguably
the most useless feature of *mms*.

### Tile Rows and Columns

Tile text is actually a somewhat expensive feature. That is, it takes a lot of
time for the computer to update and draw tile text. In order to minimize the
amount of time the computer spends performing these operations, we give
algorithm writers the ability to specify the maximum number of rows and columns
of tile text that they want displayed on the tile. By restricting the number of
rows and columns of text to only what is necessary, we can make sure
performance stays good. For information on how to set these values, check out
`tileTextNumberofRows()` and `tileTextNumberofCols()` in [Mouse Algorithm
Options](Mouse Algorithm Options).

User-specified text is automatically wrapped and trucated to fit the tile text
dimensions. For example, suppose the number of rows and columns of tile text is
two and three, respectively. If you set the text of a tile to `"foobar"`, the
text will appear as:

```
foo
bar
```

However, if you set the text of a tile to `"foo bar"`, the text will appear as:

```
foo
 ba
```

The default values for the rows and columns is two and three, respectively. We
request two rows of tile text so that [Tile Distances](Tile Distances) can be
displayed without needing to hide the user-specified tile text. We request
three columns of tile text so that all the maximum tile distance (255, or
something like that) can be displayed. That being said, these numbers are
customizable for a reason - use as many rows and columns of tile text as you
need.

### Tile Text Border

Tile text has a border around with it; the characters are easier to read when
they're not touching the boundary of the tile. The `tile-text-border-fraction`
[parameter](Parameters) controls the fraction of the tile to reserve for the
border. That is, if the fraction if `.25` Then only the middle 50% of the tile
will be used for tile text - there will be a `(.25 * wall-length)` sized border
on all sides of the tile text.

### Tile Distance

There's an [algorithm option](Mouse Algorithm Options) called
`setTileTextWhenDistanceDeclared()`that makes it so that whenever you declare a
tile's distance, it also sets the user-specified text of that tile to the
declared distance. See [Tile Distance](Tile Distance) for more information.

### Tile Text Fonts

The tile text characters are just part of a big texture, which is loaded from
an image file. The name of the image file is given by the
`tile-text-font-image` [parameter](Parameters). If you wish to change the font
image (few people have a good reason do this), you need to do a few things:

- Download some monospaced font `.ttf`
- Use the `util/ttf2png.py` script to generate a `.png` image for that font
- Move the `.png` image to the `res/imgs/` directory.
- Change the `tile-text-font-image` parameter to be the name of the `.png` file

To test the new font to see if it looks good, you can use the `FontTest`
algorithm. It automatically sets the text of the first few tiles to include all
of the possible tile text characters, and allows you to manually control the
mouse via the arrow keys. I recommend that you set the
`collision-detection-enabled` parameter to false so that you don't have to
worry about crashing while driving around.

### Examples

###### `FloodFill`

`FloodFill` shows the mouse's perceived distance-from-the-center for each tile.
As the mouse explores the maze and gathers more information, it recalculates
the distance-from-the-center for the tiles, and updates the text accordingly.

###### `MackAlgo`

`MackAlgo` shows the mouse's perceived cost-to-the-center for each tile.
That is, each time the mouse moves and gains wall information, it calculates the
cost to get from every tile to the center of the maze. The cost is a function
of number of turns and length of straightaways. When it updates the
cost-to-the-center for all of the tiles, it also updates the tile text to
display the new costs. Cool!
