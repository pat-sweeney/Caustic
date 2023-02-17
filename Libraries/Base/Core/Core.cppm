//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <string>
#include <stringapiset.h>
#include <memory>
export module Base.Core.Core:Part1;

// Namespace: Caustic
namespace Caustic
{
    //**********************************************************************
    // Function: GetCausticRootDirectory
    // Returns the default folder for the Caustic library
    //**********************************************************************
    export std::wstring GetCausticRootDirectory()
    {
        return std::wstring(CAUSTIC_ROOT_DIRECTORY);
    }

    static std::wstring _defaultShaderDirectory(CAUSTIC_SHADER_DIRECTORY);

    //**********************************************************************
    // Function: SetDefaultCausticShaderDirectory
    // Returns the default folder for the Caustic shaders
    //**********************************************************************
    export void SetDefaultCausticShaderDirectory(const wchar_t *pDefaultShaderDir)
    {
        _defaultShaderDirectory = std::wstring(pDefaultShaderDir);
    }

    //**********************************************************************
    // Function: GetCausticShaderDirectory
    // Returns the default folder for the Caustic shaders
    //**********************************************************************
    export std::wstring GetCausticShaderDirectory()
    {
#pragma warning(push)
#pragma warning(disable: 4996)
        const char* pCausticShaderDirectory = std::getenv("CausticShaderDirectory");
#pragma warning(pop)
        if (pCausticShaderDirectory != nullptr)
        {
            int numWideChars = MultiByteToWideChar(CP_UTF8, 0, pCausticShaderDirectory, -1, nullptr, 0);
            if (numWideChars > 0)
            {
                std::unique_ptr<wchar_t> pConvertedStr(new wchar_t[numWideChars]);
                if (MultiByteToWideChar(CP_UTF8, 0, pCausticShaderDirectory, -1, pConvertedStr.get(), numWideChars) != 0)
                {
                    return std::wstring(pConvertedStr.get());
                }
            }
        }
        return _defaultShaderDirectory;
    }
}
