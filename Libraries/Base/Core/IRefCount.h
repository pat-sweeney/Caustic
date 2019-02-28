//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Core.h"

namespace Caustic
{
    //**********************************************************************
    // Struct: IRefCount
	// Defines the basic interface used by reference counted objects
    //**********************************************************************
    struct IRefCount
    {
		//**********************************************************************
		// Method: AddRef
		// Increments the reference count
		//**********************************************************************
		virtual uint32 AddRef() = 0;

		//**********************************************************************
		// Method: Release
		// Decrements the reference count. If the reference count goes to zero
		// the underlying object is destroyed.
		//**********************************************************************
		virtual uint32 Release() = 0;
    };

    //**********************************************************************
    // Struct: CRefObj<T>
	// CRefObj is a smart class (similar to CComPtr) for managing
    // a IRefCount-ed object's lifetime.
	//
	// Parameters:
	// T - type of object to manage
    //**********************************************************************
    template <typename T>
    struct CRefObj
    {
        T *p;
    public:
		//**********************************************************************
		// Constructor: CRefObj
		// Default constructor
		//**********************************************************************
		CRefObj()
        {
            p = nullptr;
        }

		//**********************************************************************
		// Constructor: CRefObj
		// Copy constructor
		//**********************************************************************
		CRefObj(const CRefObj &v)
        {
            p = v.p;
            if (p)
                p->AddRef();
        }

		//**********************************************************************
		// Constructor: CRefObj
		// Constructor from base type
		//**********************************************************************
		CRefObj(T *v)
        {
            p = v;
            if (p)
                p->AddRef();
        }

		//**********************************************************************
		// Destructor: ~CRefObj
		// Destructor
		//**********************************************************************
		~CRefObj()
        {
            if (p)
                p->Release();
        }

        T *operator*() { return p; }
        T **operator&() { return &p; }
        T *operator->() { return p; }
        bool operator==(T *v) { return (p == v) ? true : false; }
        bool operator!=(T *v) { return !(p == v); }
        CRefObj &operator=(const CRefObj &v)
        {
            *this = v.p;
            return *this;
        }

        CRefObj &operator=(T *v)
        {
            if (p)
                p->Release();
            p = v;
            if (p)
                p->AddRef();
            return *this;
        }

        T *Detach() { T *q = p; p = nullptr; return q; }
    };
}