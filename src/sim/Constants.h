#pragma once

#include <GL/freeglut.h>

namespace sim {

// Simulation constants
enum {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

// Graphics constants
static GLfloat GREEN     [3] = { 0.0f, 0.9f, 0.0f};
static GLfloat DARKGREEN [3] = { 0.0f, 0.5f, 0.0f};
static GLfloat BLUE      [3] = { 0.0f, 0.0f, 0.7f};
static GLfloat GRAY      [3] = { 0.4f, 0.4f, 0.4f};
static GLfloat BLACK     [3] = { 0.0f, 0.0f, 0.0f};
static GLfloat RED       [3] = { 0.6f, 0.0f, 0.0f};

} // namespace sim
