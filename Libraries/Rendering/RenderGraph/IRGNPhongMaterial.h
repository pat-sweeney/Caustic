//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
#include "IRenderGraphNode.h"
#include "ILightCollection.h"

namespace Caustic
{
    //**********************************************************************
    // Interface: IRenderGraphNode_PhongMaterial
    // Defines interface for a material that represents the Phong Lighting model
    // in our rendergraph.
    //**********************************************************************
    struct IRenderGraphNode_PhongMaterial : public IRenderGraphNode
    {
    };
}
