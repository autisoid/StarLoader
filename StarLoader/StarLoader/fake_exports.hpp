/**
 * Copyright - xWhitey, 2025.
 * HIDDEF.H - description
 *
 * StarLoader (an .asi loader for GoldSrc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef HIDDEF_H_RECURSE_GUARD
#error Recursive header files inclusion detected in HIDDEF.H
#else //HIDDEF_H_RECURSE_GUARD

#define HIDDEF_H_RECURSE_GUARD

#ifndef HIDDEF_H_GUARD
#define HIDDEF_H_GUARD
#pragma once

void LoadRealHidDll();

extern HMODULE g_hRealHIDDll;

#endif //HIDDEF_H_GUARD

#undef HIDDEF_H_RECURSE_GUARD
#endif //HIDDEF_H_RECURSE_GUARD