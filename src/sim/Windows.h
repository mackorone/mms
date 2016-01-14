// This helps to consolidate some of the weird windows logic

// min and max are defined in Windef.h, included in windows.h
#define NOMINMAX

// Now actually include <windows.h>
#include <windows.h>

// ABSOLUTE and REALTIVE are defined in wingdi.h, included in windows.h
#undef ABSOLUTE
#undef RELATIVE
