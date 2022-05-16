//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Rendering.RenderGraph.IRGNGroup;
import Base.Core.Core;
import Base.Core.IRefCount;
import Rendering.RenderGraph.IRenderGraphNode;
import Rendering.RenderGraph.ILightCollection;

export namespace Caustic
{
    struct IRenderGraphNode_Group : public IRenderGraphNode
    {
        virtual void AddChild(IRenderGraphNode* pChild) = 0;
        virtual CRefObj<IRenderGraphPin> AddInputPin(std::string pinName, ERenderGraphDataType dataType) = 0;
    };
}
