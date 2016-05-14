Tile fog is a mechanism for determining exactly where the mouse has and hasn't
been. It's implemented as a partially transparent polygon that's overlayed over
each tile.

Tile fog is automatically cleared whenever the mouse enters a new tile. This
should be sufficient for most users, but if you wish to have the mouse
algorithm control the tile fog, you can indicate such by overriding the
`MouseAlgorithm::automaticallyClearTileFog()` method to return false, like:

```c++
// In the *.h file
bool automaticallyClearTileFog();

// In the *.cpp file
bool MyMouseAlgorithm::automaticallyClearFog() const {
    return false;
}
```

Then, fog can be controlled programatically as follows:

```c++
// Makes the tile located at position (0,0) foggy
mouse->setTileFogginess(0, 0, true);

// Clears the tile fog from the tile located at position (0,0)
mouse->setTileFogginess(0, 0, false);

// Prints a warning and does nothing, since there is not tile at position (-1, -1)
mouse->setTileFogginess(-1, -1, false);
```
