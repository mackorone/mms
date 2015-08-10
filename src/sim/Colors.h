#pragma once

#include <glut.h>
#include <map>

namespace sim {

static const GLfloat BLACK  [3] = { 0.0f, 0.0f, 0.0f};
static const GLfloat BLUE   [3] = { 0.0f, 0.0f, 0.7f};
static const GLfloat CYAN   [3] = { 0.0f, 0.4f, 0.4f};
static const GLfloat GRAY   [3] = { 0.7f, 0.7f, 0.7f};
static const GLfloat GREEN  [3] = { 0.0f, 0.7f, 0.0f};
static const GLfloat ORANGE [3] = { 0.7f, 0.4f, 0.0f};
static const GLfloat RED    [3] = { 0.8f, 0.0f, 0.0f};
static const GLfloat WHITE  [3] = { 1.0f, 1.0f, 1.0f};
static const GLfloat YELLOW [3] = { 0.7f, 0.7f, 0.0f};
static const GLfloat DARK_BLUE   [3] = { 0.0f, 0.0f, 0.2f};
static const GLfloat DARK_CYAN   [3] = { 0.0f, 0.2f, 0.2f};
static const GLfloat DARK_GRAY   [3] = { 0.1f, 0.1f, 0.1f};
static const GLfloat DARK_GREEN  [3] = { 0.0f, 0.3f, 0.0f};
static const GLfloat DARK_RED    [3] = { 0.3f, 0.0f, 0.0f};
static const GLfloat DARK_YELLOW [3] = { 0.1f, 0.1f, 0.0f};

static const std::map<std::string, const GLfloat*> STRING_TO_COLOR = {
    {"BLACK", BLACK},
    {"BLUE", BLUE},
    {"CYAN", CYAN},
    {"GRAY", GRAY},
    {"GREEN", GREEN},
    {"ORANGE", ORANGE},
    {"RED", RED},
    {"WHITE", WHITE},
    {"YELLOW", YELLOW},
    {"DARK_BLUE", DARK_BLUE},
    {"DARK_CYAN", DARK_CYAN},
    {"DARK_GRAY", DARK_GRAY},
    {"DARK_GREEN", DARK_GREEN},
    {"DARK_RED", DARK_RED},
    {"DARK_YELLOW", DARK_YELLOW},
};

static const std::map<char, const GLfloat*> CHAR_TO_COLOR = {
    {'k', BLACK},
    {'b', BLUE},
    {'a', GRAY},
    {'c', CYAN},
    {'g', GREEN},
    {'o', ORANGE},
    {'r', RED},
    {'w', WHITE},
    {'y', YELLOW},
    {'B', DARK_BLUE},
    {'C', DARK_CYAN},
    {'A', DARK_GRAY},
    {'G', DARK_GREEN},
    {'R', DARK_RED},
    {'Y', DARK_YELLOW},
};

} // namespace sim
