//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>

export module Base.Core.RefCount;
import Base.Core.Core;

export namespace Caustic
{
    //**********************************************************************
    // Class: CRefCount 
	// Defines a base class that all our reference counted
    // objects are derived from. When an obejcts reference count goes to 0
    // the object will be deleted.
    //
    // Module:
    // {Link:import Base.Core.RefCount;{Base/Core/RefCount.ixx}}
    //**********************************************************************
    class CRefCount
    {
    protected:
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
