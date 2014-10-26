Line Length
-----------

// Lines should not exceed 100 characters in length. Lines longer than 100 characters should be
// broken up into two lines.


Braces
------

// Opening braces should appear on the same line of a class/conditional/function header. Closing
// braces should appear on their own line. There should a be a single space before every opening
// brace.

void foo() {
}

class Bar {
}

if (x < 4) {
}

while (i < myList.size()) {
}


Parameter/Argument Lists
------------------------

// Parameter/Argument lists should follow arguments with a comma and single space.

void foo(int one, int two, std::string three) {
}

// Long parameter/argument lists should be broken into multiple lines. The first part of
// the second line should align with the first part of the first line.

void foo(int one, int two, std::string three, std::string four, long five, bool six,
         long long seven, std::string eight) {
}

std::cout << "Here is a really long sentence - a run on if you will, and one that you really "
          << "shouldn't write in an English class for sure." << std::endl;


Namespaces
----------

// User-defined namespaces should be used where appropriate. Namespaces do not force an additional
// level of indentation.

// Namespaces should be fully qualified, though there may be infrequent exceptions to this rule.


Comments
----------

// Comments should be used abundantly to describe the intention of the code. Single line comments
// are preferred over multi-line comments.

// Descriptions of functions, variables, etc. should be placed in the ".h" files, over the thing
// that they describe

// This does blah blah
void foo(int a, int b);


Identifiers
------------

// Identifiers should be chosen thoughtfully and carefully. Names should not be abbreviated unless
// it is abundantly clear what the abbreviation means.


Naming Conventions
------------------

// Local variables should use camelcase notation

int someVariable;

// Functions/Methods should also use camelcase notation

void fooBar();

// Classes should also use camelcase, but begin with a capital letter

class GreatClass;

// Instance/Member variables should be prefixed with "m_"

int m_highScore;


Declaration/Initialization
--------------------------

// If possible, always intialize variables when you declare them

int myInteger = 0;


Initialization Lists
--------------------

// Initialization lists are always preferred to performing initialization in the body of a
// class's constructor

Foo::Foo(int one, int two, int three) : m_one(one), m_two(two) {
    m_three = three; // Don't do this
}


Indentation
-----------

// One level of indentation equals four spaces. No more, no less, and certainly no tab characters.


Pass by Reference vs Pass by Value vs Passing a Pointer
-------------------------------------------------------

// Pass primitives by value. Pass objects by const reference. Never pass by non-const reference.
// If an argument *needs* to be modified from within a function (which shouldn't happen frequently),
// pass a pointer to the object.

int myFoo(int bar) {
}

int myBar(const Bar& bar) {
}

int modBar(Bar* bar) {
}


Placement of the Pointer / Placement of the Address-Of Operator
---------------------------------------------------------------

// The pointer goes next to the type name

Bar* bar = NULL;

int foo(Dog* doggie) {
}

// The address-of operator goes next to the type name

int foo(const Bar& myBar) {
}


Casting
-------

// Avoid using casts (it's a sign that the code needs to be cleaned up). However, if you need to
// cast, avoid using C casts. Instead, use C++ casts.

int bad = (int) getLong(); // Don't do this
int good = static_cast<int>(getLong()); // Do this instead
