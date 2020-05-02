//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#ifdef TURN_ON_DIAGNOSTICS
#define SpewWarning(msg, ...)\
do {\
    wchar_t buffer[1024];\
    swprintf_s(buffer, __VA_ARGS__);\
    OutputDebugString(buffer);\
    OutputDebugString(L"\n");\
} while(0)
#else // TURN_ON_DIAGNOSTICS
#define SpewWarning(...)
#endif // TURN_ON_DIAGNOSTICS
