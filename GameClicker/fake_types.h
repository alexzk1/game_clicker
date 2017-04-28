#ifndef FAKE_TYPES_H
#define FAKE_TYPES_H

#include <stdint.h>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <winable.h>
#else
using DWORD    = uint32_t;
using HWND     = void*;
using COLORREF = uint32_t;
#define CLR_INVALID 0xFFFFFFFF
using HMODULE  = void*;

#endif


#endif // FAKE_TYPES_H
