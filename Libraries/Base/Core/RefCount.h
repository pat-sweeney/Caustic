//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Core.h"
#include <Windows.h>

namespace Caustic
{
    //**********************************************************************
    // Class: CRefCount 
	// Defines a base class that all our reference counted
    // objects are derived from. When an obejcts reference count goes to 0
    // the object will be deleted.
    //**********************************************************************
    class CRefCount
    {
        ULONG m_RefCnt; // Current reference count
    public:
        CRefCount() :
            m_RefCnt(0)
        {
        }
            
        virtual ~CRefCount() {}
        
        virtual uint32 AddRef()
        {
            return InterlockedIncrement(&m_RefCnt);
        }
        
        virtual uint32 Release()
        {
            ULONG refcnt = InterlockedDecrement(&m_RefCnt);
            if (m_RefCnt == 0)
            {
                delete this;
            }
            return refcnt;
        }
    };
};
