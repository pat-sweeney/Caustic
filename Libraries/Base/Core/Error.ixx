//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <exception>
#include <Windows.h>
#include <string>
export module Base.Core.Error;

export namespace Caustic
{
    //**********************************************************************
    // Class: CausticException
    // Class for handling throwing of exceptions
    //
    // Module:
    // {Link:import Base.Core.Error;{Base/Core/Error.ixx}}
    //**********************************************************************
    class CausticException : public std::exception
    {
        HRESULT m_hr;
        std::string m_fn;
        int m_line;
    public:
        //**********************************************************************
        // Constructor: CausticException
        // Constructor
        //
        // hr - HRESULT to throw
        // pFilename - Name of source file where exception was thrown from
        // line - Line number in source file where exception was thrown from
        //**********************************************************************
        CausticException(HRESULT hr, const char *pFilename, int line)
        {
            m_hr = hr;
            m_fn = pFilename;
            m_line = line;
        }
    };
};

//**********************************************************************
// Macro: CT
// Macro for testing result from function and throwing an exception if the result is an error
//
// hr - HRESULT to throw
//
// Module:
// {Link:import Base.Core.Error;{Base/Core/Error.ixx}}
//**********************************************************************
export inline void CT(HRESULT hr)
{
    HRESULT _hr = (hr);
    if (FAILED(_hr))
        throw ::Caustic::CausticException(hr, __FILE__, __LINE__);
}

//**********************************************************************
// Macro: CTWIN32
// Macro for calling a Win32 function and throwing if it fails
//
// func - function to call
//
// Module:
// {Link:import Base.Core.Error;{Base/Core/Error.ixx}}
//**********************************************************************
export inline void CTWIN32(bool func)
{
    if (!(func))
        throw ::Caustic::CausticException(HRESULT_FROM_WIN32(GetLastError()), __FILE__, __LINE__);
}
