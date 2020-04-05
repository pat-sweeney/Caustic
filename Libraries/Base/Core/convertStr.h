//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "error.h"
#include <math.h>
#include <string>
#include <stringapiset.h>
#include <memory>

#ifdef CAUSTIC_EXPORT
#define CAUSTICAPI __declspec(dllexport)
#else
#define CAUSTICAPI extern
#endif

// Namespace: Caustic
namespace Caustic
{
    extern std::wstring str2wstr(const std::string& str);
    extern std::string wstr2str(const std::wstring& wstr);
}
