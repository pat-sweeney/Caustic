//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\error.h"
#include "RenderGraphNode.h"
#include "ILightCollection.h"
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // Class: CLightCollection
    // Simple class that implements <ILightCollection>. The whole point
    // of this class is so that we can store a std::vector<> as an object
    // in our render graph without having to copy it (i.e. don't need to place
    // the std::vector<> into std::any) or treat it as a unmanaged pointer.
    //**********************************************************************
    class CLightCollection : public ILightCollection, public CRefCount
    {
        std::vector<CRefObj<ILight>> m_lights;
    public:
        CLightCollection()
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ILightCollection
        //**********************************************************************
        virtual void AddLight(ILight* pLight) override { m_lights.push_back(CRefObj<ILight>(pLight)); }
        virtual void Clear() override { m_lights.clear(); }
        virtual std::vector<CRefObj<ILight>> ToVector() override { return m_lights; }
    };
}