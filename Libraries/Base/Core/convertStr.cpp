//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Error;
#include <exception>
#include <Windows.h>
#include <string>
#include <math.h>
#include <string>
#include <stringapiset.h>
#include <memory>

#ifdef CAUSTIC_EXPORT
#define __declspec(dllexport)
#else
#define extern
#endif

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Function: str2wstr
    // Converts a UTF8 string into a UTF16 string
    //
    // Parameters:
    // str - String to convert
    //
    // Returns:
    // Returns the converted string
    //
    // Header:
    // [Link:Base/Core/convertStr.h]
    //**********************************************************************
    std::wstring str2wstr(const std::string& str)
    {
        int numWideChars = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (numWideChars == 0)
            CT(E_FAIL);
        std::unique_ptr<wchar_t> pConvertedStr(new wchar_t[numWideChars]);
        if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pConvertedStr.get(), numWideChars) == 0)
            CT(E_FAIL);
        return std::wstring(pConvertedStr.get());
    }

    //**********************************************************************
    // Function: wstr2str
    // Converts a UTF16 string into a UTF8 string
    //
    // Parameters:
    // str - String to convert
    //
    // Returns:
    // Returns the converted string
    //
    // Header:
    // [Link:Base/Core/convertStr.h]
    //**********************************************************************
    std::string wstr2str(const std::wstring& wstr)
    {
        int numMultibyteChars = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (numMultibyteChars == 0)
            CT(E_FAIL);
        std::unique_ptr<char> pConvertedStr(new char[numMultibyteChars]);
        if (WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pConvertedStr.get(), numMultibyteChars, nullptr, nullptr) == 0)
            CT(E_FAIL);
        return std::string(pConvertedStr.get());
    }
}
