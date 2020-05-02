//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "ILightCollection.h"
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // Interface: IRenderGraphNode_LightCollection
    // Defines interface for adding lights to a collection
    //**********************************************************************
    struct IRenderGraphNode_LightCollection : public IRenderGraphNode
    {
        virtual void AddLight(IPointLight* pLight) = 0;
        virtual void Clear() = 0;
        virtual std::vector<CRefObj<IPointLight>> ToVector() = 0;
        virtual CRefObj<ILightCollection> GetLights() = 0;
    };
}
