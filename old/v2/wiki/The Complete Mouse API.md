### Functions that will work with any mouse

```c++
// Logging functions
void debug(const std::string& str);
void info(const std::string& str);
void warn(const std::string& str);
void error(const std::string& str);

// Returns a random number in the interval [0.0, 1.0)
double getRandom();

// # of milliseconds of sim time (adjusted based on sim speed) that have passed
int millis();

// # of milliseconds of sim time (adjusted based on sim speed)
void delay(int milliseconds);

// Forces the simulator to quit
void quit();

// Tile color
void setTileColor(int x, int y, char color);
void clearTileColor(int x, int y);
void clearAllTileColor();

// Tile text
void setTileText(int x, int y, const std::string& text);
void clearTileText(int x, int y);
void clearAllTileText();

// Tile walls
void declareWall(int x, int y, char direction, bool wallExists);
void undeclareWall(int x, int y, char direction);

// Tile fog
void setTileFogginess(int x, int y, bool foggy);

// Tile distance, where a negative distance corresponds to inf distance
void declareTileDistance(int x, int y, int distance);
void undeclareTileDistance(int x, int y);

// Reset position of the mouse
void resetPosition();

// Input buttons
bool inputButtonPressed(int inputButton);
void acknowledgeInputButtonPressed(int inputButton);
```

### CONTINUOUS Mouse Functions

```c++
// Get the magnitude of the max speed of any one wheel in rpm
double getWheelMaxSpeed(const std::string& name);

// Set the speed of any one wheel
void setWheelSpeed(const std::string& name, double rpm);

// Get the number of encoder ticks per revolution for a wheel
double getWheelEncoderTicksPerRevolution(const std::string& name);

// Read the encoder for a particular wheel
int readWheelEncoder(const std::string& name);

// Reset the encoder for a particular wheel to zero, but only if the encoder
// type is relative
void resetWheelEncoder(const std::string& name);

// Returns a value in [0.0, 1.0]
double readSensor(std::string name);

// Returns deg/s of rotation
double readGyro();
```

### DISCRETE Mouse Functions

#### Any DISCRETE Functions

These functions can be used by any mouse that uses the `DISCRETE` interface

```c++
// Returns true if there's a wall in front of the mouse, false otherwise
bool wallFront();

// Returns true if there's a wall to the right of the mouse, false otherwise
bool wallRight();

// Returns true if there's a wall to the left of the mouse, false otherwise
bool wallLeft();
```

#### Basic DISCRETE Functions

These functions are the most basic movement functions, and can only be used by
mouse algorithms that return `false` from the mouse algorithm option
`useTileEdgeMovements()`

```c++
void moveForward(); // Move forward one tile length
void moveForward(int count); // Move forward count tile lengths

void turnLeft(); // Turn ninty degrees to the right
void turnRight(); // Turn ninty degrees to the left

void turnAroundLeft(); // Turn one hundred eighty degrees to the left
void turnAroundRight(); // Turn one hundred eighty degrees to the right
```

#### Special DISCRETE Functions

These functions are the most special movement functions, and can only be used by
mouse algorithms that return `true` from the mouse algorithm option
`useTileEdgeMovements()`

```c++
// You should only call these methods at the very beginning, within the
// starting tile
// ====================================================================

void originMoveForwardToEdge();
void originTurnLeftInPlace();
void originTurnRightInPlace();

// You should only call these methods once outside of the starting tile
// ====================================================================

// Moves forward one tile length
void moveForwardToEdge();

// Moves forward count tile lengths
void moveForwardToEdge(int count);

// Performs a left curve turn
void turnLeftToEdge();

// Performs a right curve turn
void turnRightToEdge();

// Turns the mouse completely around, turns to the left
void turnAroundLeftToEdge();

// Turns the mouse completely around, turns to the right
void turnAroundRightToEdge();

// Traverses count tiles diagonally, starting and ending by turning left
void diagonalLeftLeft(int count);

// Traverses count tiles diagonally, starting by turning left and ending by
// turning right
void diagonalLeftRight(int count);

// Traverses count tiles diagonally, starting by turning right and ending by
// turning left
void diagonalRightLeft(int count);

// Traverses count tiles diagonally, starting and ending by turning right
void diagonalRightRight(int count);
```

### Omniscience Functions

The functions can only be used if your algorithm overrides
`IMouseAlgorithm::allowOmniscience()` to return `true`. These methods return
the information that's currently provided in the window header, which wouldn't
normally be available to a mouse during its run.

```c++
int currentXTile();
int currentYTile();
char currentDirection();

double currentXPosMeters();
double currentYPosMeters();
double currentRotationDegrees();
```
