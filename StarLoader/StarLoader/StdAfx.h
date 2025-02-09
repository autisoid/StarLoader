/**
 * Copyright - xWhitey, 2025.
 * StdAfx.h - our precompiled header
 *
 * StarLoader (an .asi loader for GoldSrc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef STDAFX_H_RECURSE_GUARD
#error Recursive header files inclusion detected in StdAfx.h
#else //STDAFX_H_RECURSE_GUARD

#define STDAFX_H_RECURSE_GUARD

#ifndef STDAFX_H_GUARD
#define STDAFX_H_GUARD
#pragma once

#ifdef __cplusplus

#include "common.hpp"

#else //!__cplusplus
#error C++ compiler required to compile StdAfx.h
#endif //__cplusplus

#endif //STDAFX_H_GUARD

#undef STDAFX_H_RECURSE_GUARD
#endif //STDAFX_H_RECURSE_GUARD

#pragma hdrstop