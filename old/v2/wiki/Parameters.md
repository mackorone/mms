There are a plethora of different parameters for the *mms*. They are currently
located in `res/parameters.xml`, and each falls into one of four categories:

1. Graphical parameters
2. Simulation parameters
3. Maze parameters
4. Mouse parameters

The values of the parameters can be be modified by simply editing the text file
`res/parameters.xml`. The new parameter values will take effect the next time
the simulation is run - no need to compile.

All of the parameters are validated at runtime. If you specify an invalid
parameter value, a warning will print out and the default value for that
parameter will be used instead. The valid and default values for all parameters
can be found in `src/src/ParamParser.cpp`.

If the type of the parameter is hard to determine, just do your best to infer
the type based on the current values. If you're wrong, the simulator will
complain and correct you. There are plans to add type information to all
parameters, but I'm not sure how soon anyone will get around to doing that.

To see a full list of valid colors, take a peek at `src/sim/Color.h`.

Lastly, many of these parameters should be good as is. Beginners should
really have to touch `mouse-algorithm`. That being said, they exist so that
advanced users can tweak the system however they see fit.

### Graphical parameters

###### `default-window-width`
The default width of the simulator window, in pixels

###### `default-window-height`
The default height of the simulator window, in pixels

###### `default-layout-type`
The default layout type, can be `BOTH`, `FULL`, or `ZOOMED`

###### `window-border-width`
The width of the border between components in the simulator window

###### `header-text-font`
The font to use for header text, relative to `res/fonts`

###### `header-text-height`
The height of the header text, in units a little bit larger than pixels

###### `header-row-spacing`
The number of pixels between rows of header text

###### `header-column-spacing`
The number of pixels between columns of header text

###### `min-zoomed-map-scale`
The minimum scale of the zoomed map, i.e., max amount you can zoom out

###### `max-zoomed-map-scale`
The maximum scale of the zoomed map, i.e., max amount you can zoom in

###### `default-zoomed-map-scale`
The default scale of the zoomed map

###### `default-rotate-zoomed-map`
Whether or not to rotate the zoomed map when the mouse rotates

###### `frame-rate`
The frame rate for the simulator graphics in frames per second

###### `print-late-frames`
Whether or not to print out warnings when frames are late to be displayed

###### `tile-base-color`
The color of the tile base

###### `tile-wall-color`
The color of the tile walls

###### `tile-corner-color`
The color of the tile corners

###### `tile-fog-color`
The color of the tile fog

###### `tile-text-font-image`
The image containing tile text font characters

###### `tile-text-border-fraction`
The fraction of a tile to reserve for a border around the tile text

###### `tile-text-alignment`
The alignment of the text in a tile, see `src/sim/TileTextAlignment.h` for more
info

###### `tile-undeclared-wall-color`
The color of an undeclared tile wall

###### `tile-undeclared-no-wall-color`
The color of a tile wall that has yet to be declared as non-existent

###### `tile-incorrectly-declared-wall-color`
The color of a tile wall if incorrectly declared to exist

###### `tile-incorrectly-declared-no-wall-color`
The color of a tile wall if incorrectly declared as non-existent

###### `mouse-body-color`
The color of the mouse body

###### `mouse-center-of-mass-color`
The color of the center of mass of the mouse

###### `mouse-wheel-color`
The color of the wheels of the mouse

###### `mouse-wheel-speed-indicator-color`
The color of the wheel speed indicator polygons (triangles in the wheels)

###### `mouse-sensor-color`
The color of the circular body of the sensors

###### `mouse-view-color`
The color of the view of the sensors

###### `default-tile-colors-visible`
Whether or not tile colors are visible by default

###### `default-wall-truth-visible`
Whether or not the wall truth is visible by default (as opposed to the mouse's
declared wall knowledge)

###### `default-tile-fog-visible`
Whether or not tile fog is visible by default

###### `default-tile-text-visible`
Whether or not tile text is visible by default

###### `default-tile-distance-visible`
Whether or not tile-to-center distance is visible by default

###### `tile-fog-alpha`
The transparency of the tile fog (0.0 means completely transparent, 1.0 means
completely opaque)

###### `default-wireframe-mode`
Whether or not the simulator defaults to wire frame graphics

###### `distance-correct-tile-base-color`
The color that the tile base is set to when the distance of the tile is
declared correctly

### Simulation parameters

###### `random-seed`
The seed value for rand(), can be used to add determinism to the simulator

###### `use-random-seed`
Whether or not to use the seed value, or to use an arbitrary seed value

###### `collision-detection-enabled`
Whether or not `CONTINUOUS` mode collision detection is on/off

###### `crash-message`
The message that should print out when the mouse crashes

###### `default-tile-text-character`
The character that is displayed on a tile instead of unknown/undisplayable
characters

###### `glut-init-duration`
The amount of time to wait, in seconds, for the window to initialize before
starting the simulation 

###### `default-paused`
Whether or not the simulator should start paused or not

###### `min-sim-speed`
The minimum fraction/factor of actual time that the simulator is allowed to run
at in `DISCRETE` mode

###### `max-sim-speed`
The maximum fraction/factor of actual time that the simulator is allowed to run
at in `DISCRETE` mode

###### `default-sim-speed`
The default fraction/factor of actual time of the simulator in `DISCRETE` mode

###### `min-sleep-duration`
The amount of time, in millisecond, that the simulator should sleep between
polling operations

###### `mouse-position-update-rate`
The number of discrete update steps that the simulator should take every second

###### `print-late-mouse-position-updates`
Whether or not to print a warning if the simulator takes longer than expected
to perform updates

###### `collision-detection-rate`
The number of times, per second, to check for a collision in `CONTINUOUS` mode

###### `print-late-collision-detections`
Whether or not to print a warning if the simulator takes longer than expected
to check for a collision

###### `number-of-circle-approximation-points`
The number of sides of the regular polygon that approximates a circle

###### `number-of-sensor-edge-points`
The number of points on the far edge of a sensor view polygon (higher means
more accurate, more expensive)

###### `number-of-archived-runs`
The number of runs to keep archived in the `mms/run/` directory

### Maze parameters

###### `wall-length`
The maze wall length, in meters

###### `wall-width`
The maze wall width, in meters

###### `maze-file`
The maze file to load, relative to maze-directory

###### `use-maze-file`
whether or not to use the maze file above (if not, it'll use the
`maze-algorithm` specified below)

###### `generated-maze-width`
Number of tiles wide of a generated maze

###### `generated-maze-height`
Number of tiles high of a generated maze

###### `maze-algorithm`
The algorithm, as specified in `src/maze/MazeAlgorithms.cpp`, to use to
generate a maze

###### `save-generated-maze`
Whether or not to save the generated maze

###### `generated-maze-file`
The name of the file of the generated maze, if saved

###### `maze-mirrored`
Whether or not to mirror the generated/loaded maze across the vertical axis,
before the rotation

###### `maze-rotations`
The number of times the generated/loaded maze should be rotated
counterclockwise, after the mirroring

### Mouse parameters

###### `mouse-algorithm`
The name of the mouse algorithm to use, as specified in
`src/mouse/MouseAlgorithms.cpp`
