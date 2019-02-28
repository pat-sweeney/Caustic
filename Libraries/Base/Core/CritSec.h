//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include <windows.h>

namespace Caustic
{
	// Class: CCritSec
	// Defines a smart object for managing a critical section
    class CCritSec
    {
    protected:
        CRITICAL_SECTION m_cs;
    public:
		// Constructor: CCritSec
		// Defines the default constructor
        CCritSec()
        {
            InitializeCriticalSection(&m_cs);
        }
        
		// Destructor: ~CCritSec
		// Defines the destructor
		~CCritSec()
        {
            DeleteCriticalSection(&m_cs);
        }

		// Method: Enter
		// Enters the critical section
		void Enter()
        {
            EnterCriticalSection(&m_cs);
        }

		// Method: Leave
		// Leaves the critical section
		void Leave()
        {
            LeaveCriticalSection(&m_cs);
        }
    };

    /// \brief CAutoLock provides smart object management of critical sections
    class CAutoLock
    {
    protected:
        CCritSec *m_pSec;
    public:
        CAutoLock(CCritSec *pLock)
        {
            m_pSec = pLock;
            m_pSec->Enter();
        }
        
        ~CAutoLock()
        {
            m_pSec->Leave();
        }
    };

};
