Tile distance is a feature that makes it easier for flood-fill like algorithms
to use [Tile Color](Tile Color) and [Tile Text](Tile Text) in intuitive ways.
It serves three primary purposes:

- Automatically sets the tile text to the declared distance
- Automatically sets the tile color when the declared distance is correct
- Allows the user to see the actual distance-to-the-center for each tile

The API for tile distance is as follows:

```c++
// Declare the distance of the tile at location (x, y), where a negative value
// for distance is interpreted to "infinite" (the tile text will reflect this)
void declareTileDistance(int x, int y, int distance);

// Undo a tile distance declaration, which removes tile text if
// setTileTextWhenDistanceDeclared() is defined to return true, and removes
// tile color if setTileBaseColorWhenDistanceDeclaredCorrectly() is defined
// to return true
void undeclareTileDistance(int x, int y);
```

`undeclareTileDistance()` is just a way to tell the simulator that you are no
longer confident about the tile distance, which is different than declaring the
distance to be zero (or some other value).


### `setTileTextWhenDistanceDeclared()`

When this [mouse algorithm option](Mouse Algorithm Options) is defined to
return to `true` (which it is by default), the tile text is set to the declared
distance, if and when the tile distance is declared. This is nice because the
int -> string conversion is done for you. As noted in [Tile Text](Tile Text),
if the length of the string of the declared distance doesn't fit within the
`tileTextNumberOfRows()` and `tileTextNumberOfCols()`, it will be wrapped and
truncated. Note that setting the tile text overwrites any text that was
previously in the tile.

If you're using this setting, chances are you shouldn't be using the tile text
API functions at all. Instead, you should just let the simulator do the work of
setting the tile text for you.

### `setTileBaseColorWhenDistanceDeclaredCorrectly()`

When this [option](Mouse Algorithm Options) is defined to return true
(by default, it is not), the tile color is set to value of the
`distance-correct-tile-base-color` [parameter](Parameters) if and when the tile
distance is declared correctly. This means that if you declare a tile distance
and it is incorrect, the tile color will not change. But if you declare a tile
distance and it is correct, the tile color will change, and overwrite any
preexisting tile colors.

If you're using this setting, chances are you shouldn't be using the tile color
API functions at all. Instead, you should just let the simulator do the work of
setting the tile color for you.

### Actual Tile Distance Text Visibility

In addition to algorithm-declared tile distances, the simulator also calculates
and stores the actual tile distances. The actual tile distances are displayed
as tile text. However, the actual tile distance text behaves a little bit
differently from regular tile text:

- The `default-tile-text-visible` parameter controls whether the actual tile
  distance is visible by default, provided that tile text is enabled by
  default
- The actual tile distance text does not overwrite user-controlled tile text
- The actual tile distance text can be toggled on and off by pressing the "d" button
- When the actual tile distance text is visible, it will push the existing
  tile text down one row, potentially forcing other tile text to no longer be displayed
- The actual tile distance text will never wrap, but it will truncate, if and when
  the text does not fit within the `tileTextNumberOfCols()`

So basically, pressing "d" will toggle the actual tile distance text in a
benign way; it won't overwrite the preexisting tile text, it'll just move it
down one row.
