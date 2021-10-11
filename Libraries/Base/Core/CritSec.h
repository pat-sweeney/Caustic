//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <windows.h>

namespace Caustic
{
	//**********************************************************************
	// Class: CCritSec
	// Defines a smart object for managing a critical section
	//
	// Header:
	// [Link:Base/Core/CritSec.h]
	//**********************************************************************
	class CCritSec
    {
    protected:
        CRITICAL_SECTION m_cs;
    public:
		//**********************************************************************
		// Constructor: CCritSec
		// Defines the default constructor
		//**********************************************************************
		CCritSec()
        {
            InitializeCriticalSection(&m_cs);
        }
        
		//**********************************************************************
		// Destructor: ~CCritSec
		// Defines the destructor
		//**********************************************************************
		~CCritSec()
        {
            DeleteCriticalSection(&m_cs);
        }

		//**********************************************************************
		// Method: Enter
		// Enters the critical section
		//**********************************************************************
		void Enter()
        {
            EnterCriticalSection(&m_cs);
        }

		//**********************************************************************
		// Method: Leave
		// Leaves the critical section
		//**********************************************************************
		void Leave()
        {
            LeaveCriticalSection(&m_cs);
        }
    };

	//**********************************************************************
	// Class: CAutoLock
	// Provides smart object management of critical sections
	//
	// Header:
	// [Link:Base/Core/CritSec.h]
	//**********************************************************************
	class CAutoLock
    {
    protected:
        CCritSec *m_pSec;
    public:
		//**********************************************************************
		// Constructor: CAutoLock
		// Locks the specified critical section on stack entrance
		//
		// Parameters:
		// pLock - critical section to lock
		//**********************************************************************
		CAutoLock(CCritSec *pLock)
        {
            m_pSec = pLock;
            m_pSec->Enter();
        }
        
		//**********************************************************************
		// Destructor: CAutoLock
		// Unlocks the specified critical section on stack exit
		//**********************************************************************
		~CAutoLock()
        {
            m_pSec->Leave();
        }
    };
};
