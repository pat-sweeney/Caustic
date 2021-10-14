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
    // Interface: IRenderGraphNode_Material
    // Defines interface for a material in our rendergraph.
    // This object represents a node that generates an <IRenderMaterial>
    //**********************************************************************
    struct IRenderGraphNode_Material : public IRenderGraphNode
    {
    };
}
