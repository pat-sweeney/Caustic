//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include "RenderGraphNode.h"
#include <vector>
#include <string>
#include <any>

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
