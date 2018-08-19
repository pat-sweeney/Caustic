//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <windows.h>

namespace Caustic
{
    class CCritSec
    {
    protected:
        CRITICAL_SECTION m_cs;
    public:
        CCritSec()
        {
            InitializeCriticalSection(&m_cs);
        }
        
        ~CCritSec()
        {
            DeleteCriticalSection(&m_cs);
        }

        void Enter()
        {
            EnterCriticalSection(&m_cs);
        }

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
