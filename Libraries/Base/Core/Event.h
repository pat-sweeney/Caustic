//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <Windows.h>

namespace Caustic
{
    //**********************************************************************
    // Class: CEvent
	// Defines a smart wrapper for handling events
    //**********************************************************************
    class CEvent
    {
        HANDLE m_h;
    public:
		//**********************************************************************
		// Constructor: CEvent
		// Defines the constructor for an event
		//
		// Parameters:
		//
		// defaultState - Sets the initial state of the event
		// manualReset - Specifies whether event needs to be manually reset
		//**********************************************************************
		explicit CEvent(bool defaultState, bool manualReset)
        {
            m_h = CreateEvent(nullptr, manualReset, defaultState, NULL);
        }

		//**********************************************************************
		// Destructor: ~CEvent
		// Default destructor
		//**********************************************************************
		~CEvent()
        {
            CloseHandle(m_h);
        }

		//**********************************************************************
		// Method: Set
		// Sets the event
		//
		// Returns:
		// Returns true if event was successfully set. Otherwise returns false.
		//**********************************************************************
		bool Set()
        {
            return SetEvent(m_h) ? true : false;
        }

		//**********************************************************************
		// Method: Clear
		// Clears the event
		//
		// Returns:
		// Returns true if event was successfully reset. Otherwise returns false.
		//**********************************************************************
		bool Clear()
        {
            return ResetEvent(m_h) ? true : false;
        }

		//**********************************************************************
		// Method: Wait
		// Waits till the event is signaled or times out
		//
		// Parameters:
		// dwTimeout - time in milliseconds to wait before timing out
		//
		// Returns:
		// Returns true if event was successfully reset. Otherwise returns false.
		//**********************************************************************
		DWORD Wait(DWORD dwTimeout)
        {
            return WaitForSingleObject(m_h, dwTimeout);
        }
    };
};
