//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <Windows.h>

namespace Caustic
{
    //**********************************************************************
    ///! \brief CEvent defines a smart wrapper for handling events
    //**********************************************************************
    class CEvent
    {
        HANDLE m_h;
    public:
        explicit CEvent(bool defaultState, bool manualReset)
        {
            m_h = CreateEvent(nullptr, manualReset, defaultState, NULL);
        }

        ~CEvent()
        {
            CloseHandle(m_h);
        }

        bool Set()
        {
            return SetEvent(m_h) ? true : false;
        }

        bool Clear()
        {
            return ResetEvent(m_h) ? true : false;
        }

        DWORD Wait(DWORD dwTimeout)
        {
            return WaitForSingleObject(m_h, dwTimeout);
        }
    };
};
