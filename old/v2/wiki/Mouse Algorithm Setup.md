There are a couple of things you need to do before you can start writing the
logic for your Micromouse algorithm. They are as follows:

### Create a new directory for your mouse algorithm code in `src/mouse`.

If you're on Ubuntu/Linux, you might run the following command:

```bash
mkdir src/mouse/foo
```

It's a good idea to put all of your algorithm code in its own directory so that
it doesn't get mixed up with other algorithms' code.

### Define a class that implements the `IMouseAlgorithm` interface.

For example, here's what you could put in `src/mouse/foo/Bar.h` and
`src/mouse/foo/Bar.cpp`:

**`src/mouse/foo/Bar.h`**
```c++
#pragma once

#include "../IMouseAlgorithm.h"

namespace foo {

class Bar : public IMouseAlgorithm {

public:
    void solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse);

};

} // namespace foo
```

**`src/mouse/foo/Bar.cpp`**
```c++
#include "Bar.h"

namespace foo {

void Bar::solve(
        int mazeWidth, int mazeHeight, bool isOfficialMaze,
        char initialDirection, sim::MouseInterface* mouse) {

    // TODO: Put mouse algorithm logic in here
}
```

### Add your class to the list of algorithms in `src/mouse/MouseAlgorithms.cpp`

This effectively "registers" your new algorithm with the simulator. Here's what
you would add to `MouseAlgorithms.cpp`, where the `...`s mean that there is
other code there, but we're ignoring it for now:

**`src/mouse/MouseAlgorithms.cpp`**
```c++
#include "MouseAlgorithms.h"
...
#include "foo/Bar.h"
...
std::pair<bool, IMouseAlgorithm*> MouseAlgorithms::helper(...) {
    ...
    ALGO("myFooBar", new foo::Bar());
    ...
}
```

Note that "myFooBar" is just that name that we'll use to refer to your mouse
algorithm and is completely arbitrary. The only thing that matters is that is
isn't a duplicate of a preexisting algorithm's name.

### Update the `mouse-algorithm` parameter in `res/parameters.xml`

Again, the `...`s mean that there is other code there, but we're ignoring it
for now.

**`res/parameters.xml`**
```xml
...
<!-- Mouse Parameters -->
<mouse-algorithm>myFooBar</mouse-algorithm>
...
```

By changing this parameter, you'll telling the simulator to use your algorithm
when it runs. Note that all of the parameters in `res/parameters.xml` are
runtime parameters, which means that you can change the values and re-run the
simulator again, without needing to compile.

### Build and Run

Once you've done the above four things, go ahead and build (e.g., `make` on
Linux) the project and run it. If everything worked successfully, you should
see a mouse robot that just sits in place and does nothing.

### `Solve` Method Arguments

As you may have noticed, there are five different arguments to the `solve()`
method shown above. They are as follows:

###### `int mazeWidth`

The width of the maze, in tiles

###### `int mazeHeight`

The height of the maze, in tiles

###### `bool isOfficialMaze`

Whether or not the maze is an official maze. For more information about
official mazes, check out [Official Mazes](Official Mazes).

###### `char initialDirection`

The initial direction of the mouse. Either `n`, `e`, `s`, or `w`, for north,
east, south, and west, respectively.

###### `sim::MouseInterface* mouse`

The interface object though while you control the Micromouse. See [The Complete
Mouse API](The Complete Mouse API) for more information.
