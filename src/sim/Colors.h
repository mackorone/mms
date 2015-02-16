#pragma once

#include <GL/freeglut.h>

namespace sim {

// Graphics constants
// TODO: Do these actually need to be GLfloat ???
static GLfloat GREEN     [3] = { 0.0f, 0.9f, 0.0f};
static GLfloat DARKGREEN [3] = { 0.0f, 0.5f, 0.0f};
static GLfloat BLUE      [3] = { 0.0f, 0.0f, 0.7f};
static GLfloat GRAY      [3] = { 0.4f, 0.4f, 0.4f};
static GLfloat LIGHTGRAY [3] = { 0.7f, 0.7f, 0.7f};
static GLfloat BLACK     [3] = { 0.0f, 0.0f, 0.0f};
static GLfloat WHITE     [3] = { 1.0f, 1.0f, 1.0f};
static GLfloat RED       [3] = { 0.6f, 0.0f, 0.0f};

} // namespace sim
