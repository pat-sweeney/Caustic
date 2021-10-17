//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Error;
#include <math.h>
#include <string>
#include <stringapiset.h>
#include <memory>

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Function: str2wstr
    // See <str2wstr>
    // 
    // Header:
    // {Link:#include "Base/Core/convertStr.h"{Base/Core/convertStr.h}}
    //**********************************************************************
    extern std::wstring str2wstr(const std::string& str);

    //**********************************************************************
    // Function: wstr2str
    // See <wstr2str>
    // 
    // Header:
    // {Link:#include "Base/Core/convertStr.h"{Base/Core/convertStr.h}}
    //**********************************************************************
    extern std::string wstr2str(const std::wstring& wstr);
}
