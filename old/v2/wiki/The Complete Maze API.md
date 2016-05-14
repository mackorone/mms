The maze API is very simple - only one function is provided to set wall values.
Note that you have to be careful to always set both sides of a wall to the same
value, otherwise the maze will fail validation. 

```c++
// Logging functions
void debug(const std::string& str);
void info(const std::string& str);
void warn(const std::string& str);
void error(const std::string& str);

// Misc functions
double getRandom(); // Returns a random number in [0.0, 1.0)
void quit(); // Forces the simulator to exit

// Sets the wall
void setWall(int x, int y, char direction, bool wallExists);
```
