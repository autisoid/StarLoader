/**
 * Copyright - xWhitey, 2025.
 * COMMON.HPP - The main file of our project with includes
 *
 * StarLoader (an .asi loader for GoldSrc) header file
 * Authors: xWhitey. Refer to common.hpp file for dependencies and their authors.
 * Do not delete this comment block. Respect others' work!
 */

#ifdef COMMON_HPP_RECURSE_GUARD
#error Recursive header files inclusion detected in common.hpp
#else //COMMON_HPP_RECURSE_GUARD

#define COMMON_HPP_RECURSE_GUARD

#ifndef COMMON_HPP_GUARD
#define COMMON_HPP_GUARD
#pragma once

#ifdef __cplusplus

#define MESSAGEBOX_TITLE	"StarLoader"
#define PLUGINS_DIRECTORY	"starloader"
#define LOG_FILE			"debug_log.txt"
#define STARLOADER_VERSION	"0.1a"
#define NUM_REAL_EXPORTS	43
//							^^^ this is ugly, I don't like this. Need to make a system which dynamically counts the exports, probably based on __COUNTER__

//WinAPI
//Fix of some weird bug which appears only in old MSVC toolchain.
#if _MSC_VER < 1920
#ifdef no_init_all
#undef no_init_all
#endif //no_init_all
#define no_init_all
#endif //MSC_VER < 1920

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Exclude rarely used components from Windows headers.
#define VC_EXTRALEAN		// Exclude rarely used components from Windows headers.

#include <Windows.h>
#include <conio.h>

//CRT
#include <stdio.h>
#include <stdlib.h>

extern const char* GetLastErrorAsHumanReadable(DWORD _ErrorCode);
extern unsigned long GetCurrentDir(char* _Buffer, size_t _BufferSize, bool _Log);
extern int SetCurrentDir(char* _Buffer, bool _Log);
extern void PrintToLogFile(const char* _Format, ...);

#else //!__cplusplus
#error C++ compiler required to compile common.hpp
#endif //__cplusplus

#endif //COMMON_HPP_GUARD

#undef COMMON_HPP_RECURSE_GUARD
#endif //COMMON_HPP_RECURSE_GUARD