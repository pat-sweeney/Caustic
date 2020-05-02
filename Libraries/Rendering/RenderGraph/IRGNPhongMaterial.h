//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
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
