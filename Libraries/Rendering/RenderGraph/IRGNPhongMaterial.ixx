//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.RenderGraph.IRGNPhongMaterial;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.RenderGraph.IRenderGraphNode;
import Rendering.RenderGraph.ILightCollection;

export namespace Caustic
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
