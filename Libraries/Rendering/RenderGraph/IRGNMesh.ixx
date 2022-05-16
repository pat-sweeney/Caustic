//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>

export module Rendering.RenderGraph.IRGNMesh;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Rendering.RenderGraph.IRenderGraphNode;

export namespace Caustic
{
    class IRenderGraphNode_Mesh : public IRenderGraphNode
    {
    };
}
