//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.RenderGraph.IRGNMaterial;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderMaterial;
import Rendering.RenderGraph.IRenderGraphNode;
import Rendering.RenderGraph.ILightCollection;

export namespace Caustic
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
