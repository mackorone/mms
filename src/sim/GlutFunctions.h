#pragma once

namespace sim {

struct GlutFunctions {
    void (*draw)(void);
    void (*keyPress)(unsigned char key, int x, int y);
    void (*specialKeyPress)(int key, int x, int y);
    void (*specialKeyRelease)(int key, int x, int y);
};

} // namespace sim
