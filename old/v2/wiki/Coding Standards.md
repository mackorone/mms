This is an incomplete list of some coding standards that I tried to follow when
I was coding the simulator.

#### Line Length

In general, lines should not exceed 100 characters in length. Lines longer than
100 characters should be broken up into two lines.

#### Constants

Don't use string literals for any value of type `Color`, `Direction`, `Key`,
`Layout`, or `InterfaceType`.  Instead, to provide better safety, get the
string value by using the appropriately defined map, as in:

```c++
std::string northString = STRING_TO_DIRECTION.at(Direction::NORTH);
```

#### Braces

Opening braces should appear on the same line of a class/conditional/function
header. Closing braces should appear on their own line. There should a be a
single space before every opening brace. Never omit braces.

```c++
void foo() {
}

class Bar {
}

if (x < 4) {
}

while (i < myList.size()) {
}
```

#### Parameter/Argument Lists

Parameter/Argument lists should follow arguments with a comma and single space.

```c++
void foo(int one, int two, std::string three) {
}
```

Long parameter/argument lists should be broken into multiple lines. The first
part of the second line should be indented four spaces in header files, eight
in `.cpp` files.

```c++
// .h file
void foo(int one, int two, std::string three, std::string four, long five,
    bool six, long long seven, std::string eight);

// .cpp file
void Bar::foo(int one, int two, std::string three, std::string four, long five,
        bool six, long long seven, std::string eight) {
}
```

#### Namespaces

User-defined namespaces should be used where appropriate. Namespaces do not
force an additional level of indentation.

Namespaces should be fully qualified, though there may be infrequent exceptions
to this rule.

#### Comments

Comments should be used abundantly to describe the intention of the code.

Descriptions of functions, variables, etc. should be placed in the ".h" files,
over the thing that they describe

```c++
// This does blah blah
void foo(int a, int b);
```

#### Identifiers

Identifiers should be chosen thoughtfully and carefully. Names should not be
abbreviated unless it is abundantly clear what the abbreviation means.

#### Naming Conventions

Local variables should use camelcase notation

```c++
int someVariable;
```

Functions/Methods should also use camelcase notation

```c++
void fooBar();
```

Classes should also use camelcase, but begin with a capital letter

```c++
class GreatClass;
```

Instance/Member variables should be prefixed with `m_`

```c++
int m_highScore;
```

#### Declaration/Initialization

If possible, always intialize variables when you declare them

```c++
int myInteger = 0;
```

#### Initialization Lists

Initialization lists are always preferred to performing initialization in the
body of a class's constructor

```c++
Foo::Foo(int one, int two, int three) : m_one(one), m_two(two) {
    m_three = three; // Don't do this
}
```

#### Indentation

One level of indentation equals four spaces. No more, no less, and certainly no
tab characters.

If you're performing a multi-line assignment, put the equals sign on the first
line, as in:

```c++
int reallyLongVariableNameThatReallyShouldBeSomethingElse =
    someReallyLongFunctionNameThatCouldProbablyBeShorter();
```

#### Pass by Reference vs Pass by Value vs Passing a Pointer

Pass primitives by value. Pass objects by const reference. Never pass by
non-const reference.  If an argument *needs* to be modified from within a
function (which shouldn't happen frequently), pass a pointer to the object.

```c++
int myFoo(int bar) {
}

int myBar(const Bar& bar) {
}

int modBar(Bar* bar) {
}
```

#### Placement of the Pointer / Placement of the Address-Of Operator

The pointer goes next to the type name

```c++
Bar* bar = NULL;

int foo(Dog* doggie) {
}
```

The address-of operator goes next to the type name

```c++
int foo(const Bar& myBar) {
}
```


#### Casting

Avoid using casts (it's a sign that the code needs to be cleaned up). However,
if you need to cast, avoid using C casts. Instead, use C++ casts.

```c++
int bad = (int) getLong(); // Don't do this
int good = static_cast<int>(getLong()); // Do this instead
```

#### Includes

Includes in the headers files should be organized into two groups: <...>
includes (libraries, things we didn't write) and "..." includes (things we
wrote). The first group should appear before the second, with exactly one
newline separating the groups. The groups should be alphabetized.

```c++
#include <iostream>
#include <vector>

#include "MyFileOne.h"
#include "ZooFoo.h"
```

Includes in cpp files should be organized in the same two groups, with one
exception: the header file corresponding to a particular class should appear on
its own line before any other includes, as in:

```c++
#include "MyClass.h"

#include <iostream>
#include <vector>

#include "MyFileOne.h"
#include "ZooFoo.h"
```

#### Logging

Always directly follow an `error` log with a call to `quit`. Likewise, always
make sure to log an error before calling `quit`.

```c++
L()->error("You messed up!");
SimUtilities::quit();
```

When continuing a string onto the next line, use the following format:

```c++
L()->warn(
    "This is a really long log statement, and I think that I'm going"
    " to put an interger here: %v. And here: %v. And maybe also here:"
    " %v. OK just one more: %v. I'll keep going, going, going since"
    " this is such a long log statement",
    1234,
    mazeHeight() + mazeWidth(),
    someReallyLongFunctionName(),
    3+4+5+6+7+8);
```

If you have a lot of values to substitute in, consider putting each on their
own line. Note that the string continuation begins with a space. Also note that
the closing parenthesis appears on the same line as the last argument.

#### Gotchas

Don't use `sim::P()` or `sim::S()` to initialize any static variables. The
initialization order of static variables is undefined and thus can mess with
the delicate instantiation of the Param and State objects.
