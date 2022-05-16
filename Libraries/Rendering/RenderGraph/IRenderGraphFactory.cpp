//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.RenderGraph.IRenderGraphFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.RenderGraph.RenderGraphFactory;

namespace Caustic
{
    //**********************************************************************
    // Function: CreateRenderGraphFactory
    // Creates a new RenderGraph factory. This factory is used to create nodes
    // that will be added to the render graph
    //
    // Returns:
    // New render factory object
    //**********************************************************************
    CRefObj<IRenderGraphFactory> CreateRenderGraphFactory()
    {
        return CRefObj<IRenderGraphFactory>(new CRenderGraphFactory());
    }
}
