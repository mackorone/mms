#pragma once

#include <vector>

namespace sim {

enum class Key { LEFT, RIGHT, UP, DOWN, SPACE };

static const std::vector<Key> ARROW_KEYS = {
    Key::LEFT, Key::RIGHT, Key::UP, Key::DOWN,
};

static const std::map<int, Key> INT_TO_KEY {
    { 32, Key::SPACE},
    {100, Key::LEFT},
    {102, Key::RIGHT},
    {101, Key::UP},
    {103, Key::DOWN},
};

} // namespace sim
