//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <exception>
#include <Windows.h>
#include <string>
#include <math.h>
#include <string>
#include <stringapiset.h>
#include <memory>

export module Base.Core.ConvertStr;
import Base.Core.Error;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Function: ExpandEnvironmentVariables
    // Replaces $(<name>) environment variables in a string
    //
    // Parameters:
    // pFilename - string to replace environment variables
    //
    // Returns:
    // Returns the expanded string
    //
    // Header:
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::wstring ExpandEnvironmentVariables(const wchar_t* pFilename)
    {
        std::wstring expanded;
        while (*pFilename)
        {
            if (*pFilename == L'$' && pFilename[1] == L'(')
            {
                const wchar_t* q = pFilename;
                while (*q)
                {
                    if (*q == L')')
                        break;
                    q++;
                }
                std::wstring str(pFilename);
                std::wstring varName = str.substr(2, q - &pFilename[2]);
                wchar_t buffer[1024];
                GetEnvironmentVariable(varName.c_str(), buffer, sizeof(buffer));
                expanded += std::wstring(buffer);
                pFilename = q + 1;
            }
            else
            {
                expanded += pFilename[0];
                pFilename++;
            }
        }
        return expanded;
    }

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
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::wstring str2wstr(const char* str)
    {
        int numWideChars = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
        if (numWideChars == 0)
            CT(E_FAIL);
        std::unique_ptr<wchar_t> pConvertedStr(new wchar_t[numWideChars]);
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, pConvertedStr.get(), numWideChars) == 0)
            CT(E_FAIL);
        return std::wstring(pConvertedStr.get());
    }

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
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::wstring str2wstr(const std::string& str)
    {
        return str2wstr(str.c_str());
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
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::string wstr2str(const wchar_t* wstr)
    {
        int numMultibyteChars = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        if (numMultibyteChars == 0)
            CT(E_FAIL);
        std::unique_ptr<char> pConvertedStr(new char[numMultibyteChars]);
        if (WideCharToMultiByte(CP_UTF8, 0, wstr, -1, pConvertedStr.get(), numMultibyteChars, nullptr, nullptr) == 0)
            CT(E_FAIL);
        return std::string(pConvertedStr.get());
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
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::string wstr2str(const std::wstring& wstr)
    {
        return wstr2str(wstr.c_str());
    }


    //**********************************************************************
    // Function: GetExecutablePath
    // Returns the directory where the current executable is
    //
    // Parameters:
    // str - String to convert
    //
    // Returns:
    // Path where .exe resides
    //
    // Header:
    // {Link:import Base.Core.ConvertStr;{Base.Core.ConvertStr}
    //**********************************************************************
    std::wstring GetExecutablePath()
    {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring wstr(buffer);
        return wstr.substr(0, wstr.find_last_of(L"\\/"));
    }
}
