//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include <exception>
#include <Windows.h>
#include <string>

namespace Caustic
{
	//**********************************************************************
	// Class: CausticException
	// Class for handling throwing of exceptions
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
//**********************************************************************
#define CT(hr) \
do {\
    HRESULT _hr = (hr);\
    if (FAILED(_hr))\
        throw ::Caustic::CausticException(_hr, __FILE__, __LINE__);\
} while(0)

//**********************************************************************
// Macro: CTWIN32
// Macro for calling a Win32 function and throwing if it fails
//
// func - function to call
//**********************************************************************
#define CTWIN32(func) \
do {\
    if (!(func))\
        throw ::Caustic::CausticException(HRESULT_FROM_WIN32(GetLastError()), __FILE__, __LINE__);\
} while(0)
