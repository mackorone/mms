// This helps to consolidate some of the weird windows logic

// min and max are defined in Windef.h, included in windows.h
#define NOMINMAX

// Supposedly these get rid of a lot of includes we don't want/need
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

// Now actually include <windows.h>
#include <windows.h>

// ABSOLUTE and REALTIVE are defined in wingdi.h, included in windows.h
#undef ABSOLUTE
#undef RELATIVE
