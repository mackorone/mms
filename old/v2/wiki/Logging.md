### Logging

These functions print to stdout and log to a file in the run/ directory. The
levels are somewhat arbitrary. The convention that I use is that errors should
cause a failure, while warnings should allow the program to continue.

```c++
void debug(const std::string& str);
void info(const std::string& str);
void warn(const std::string& str);
void error(const std::string& str);
```
Example:
```c++
// You can log a std::string
std::string foo("Hello World!");
mouse->warn(foo);

// Or a string literal
mouse->info("Moving forward");

// Use std::to_string() for numeric values
mouse->debug(std::to_string(4));
```
