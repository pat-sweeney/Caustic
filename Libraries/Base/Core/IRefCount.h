//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Core.h"

namespace Caustic
{
    //**********************************************************************
    //! \brief Defines the basic interface used by reference counted objects
    //**********************************************************************
    struct IRefCount
    {
        virtual uint32 AddRef() = 0;
        virtual uint32 Release() = 0;
    };

    //**********************************************************************
    //! \brief CRefObj is a smart class (similar to CComPtr) for managing
    //! a IRefCount-ed object's lifetime.
    //**********************************************************************
    template <typename T>
    struct CRefObj
    {
        T *p;
    public:
        CRefObj()
        {
            p = nullptr;
        }

        CRefObj(const CRefObj &v)
        {
            p = v.p;
            if (p)
                p->AddRef();
        }

        CRefObj(T *v)
        {
            p = v;
            if (p)
                p->AddRef();
        }

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