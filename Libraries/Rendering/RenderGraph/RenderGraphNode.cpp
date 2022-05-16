//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>
#include <windows.h>

module Rendering.RenderGraph.RenderGraphNode;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.RenderGraph.RenderGraphNode;

namespace Caustic
{
    CRefObj<IRenderGraphPin> CRenderGraphNode::FindInputPin(std::string name)
    {
        for (int i = 0; i < (int)m_inputs.size(); i++)
        {
            if (m_inputs[i]->GetName() == name)
                return m_inputs[i];
        }
        return CRefObj<IRenderGraphPin>(nullptr);
    }

    CRefObj<IRenderGraphPin> CRenderGraphNode::FindOutputPin(std::string name)
    {
        for (int i = 0; i < (int)m_outputs.size(); i++)
        {
            if (m_outputs[i]->GetName() == name)
                return m_outputs[i];
        }
        return CRefObj<IRenderGraphPin>(nullptr);
    }
}
