//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once

#include <exception>
#include <Windows.h>
#include <string>

namespace Caustic
{
    class CausticException : public std::exception
    {
        HRESULT m_hr;
        std::string m_fn;
        int m_line;
    public:
        CausticException(HRESULT hr, const char *pFilename, int line)
        {
            m_hr = hr;
            m_fn = pFilename;
            m_line = line;
        }
    };
};

#define CT(hr) \
do {\
    HRESULT _hr = (hr);\
    if (FAILED(_hr))\
        throw ::Caustic::CausticException(hr, __FILE__, __LINE__);\
} while(0)

#define CTWIN32(func) \
do {\
    if (!(func))\
        throw ::Caustic::CausticException(HRESULT_FROM_WIN32(GetLastError()), __FILE__, __LINE__);\
} while(0)
