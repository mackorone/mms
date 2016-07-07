// CPMath stands for Cross Platform Math

// TODO: upforgrabs
// I think just `#include <QtMath>` should suffice here, but we should
// double-check this on other platforms. If that ends up being the case, we can
// delete this file and just `#include <QtMath>` wherever we need it.

#include <cmath>

#undef M_PI
#define M_PI 3.14159265358979323846
#undef M_TWOPI
#define M_TWOPI (2 * M_PI)
