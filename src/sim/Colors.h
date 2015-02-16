#pragma once

#include <GL/freeglut.h>
#include <map>

namespace sim {

static const GLfloat BLACK      [3] = { 0.0f, 0.0f, 0.0f};
static const GLfloat BLUE       [3] = { 0.0f, 0.0f, 0.7f};
static const GLfloat GRAY       [3] = { 0.7f, 0.7f, 0.7f};
static const GLfloat GREEN      [3] = { 0.0f, 0.7f, 0.0f};
static const GLfloat RED        [3] = { 0.6f, 0.0f, 0.0f};
static const GLfloat WHITE      [3] = { 1.0f, 1.0f, 1.0f};

static const std::map<std::string, const GLfloat*> COLORS =
    {{"BLACK", BLACK},
    {"BLUE", BLUE},
    {"GRAY", GRAY},
    {"GREEN", GREEN},
    {"RED", RED},
    {"WHITE", WHITE}};

} // namespace sim
