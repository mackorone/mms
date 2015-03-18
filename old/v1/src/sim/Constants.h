#pragma once

#include <GL/freeglut.h>

namespace sim {

// Simulation constants
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

// Graphics constants
static GLfloat GREEN     [3] = { 0,.9, 0};
static GLfloat DARKGREEN [3] = { 0,.5, 0};
static GLfloat BLUE      [3] = { 0, 0,.7};
static GLfloat GRAY      [3] = {.4,.4,.4};
static GLfloat BLACK     [3] = { 0, 0, 0};
static GLfloat RED       [3] = {.6, 0, 0};

} // namespace sim
