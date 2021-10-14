//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.Error;
#include "IRenderGraphNode.h"
#include "RenderGraphNode.h"
#include "RGNGroup.h"

namespace Caustic
{
    CRenderGraphNode_Group::CRenderGraphNode_Group()
    {
        m_spRenderPin = CreatePin(this, false, "render", ERenderGraphDataType::Void, std::any());
    }

    std::any CRenderGraphNode_Group::GetPinValue(IRenderGraphPin* pPin, IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        // RenderGraph works on a pull model. Thus we start at the end of the graph
        // and walk backwards.
        if (pPin == m_spRenderPin)
        {
            for (size_t i = 0; i < m_children.size(); i++)
            {
                CRefObj<IRenderGraphPin> spRenderPin;
                spRenderPin = m_children[i]->FindOutputPin("render");
                if (spRenderPin)
                    m_children[i]->GetPinValue(spRenderPin, pRenderer, pRenderCtx);
            }
        }
        return std::any(nullptr); // Groups don't have outputs
    }

    void CRenderGraphNode_Group::AddChild(IRenderGraphNode* pChild)
    {
        // Add each childs input pins to our list of input pins
        for (int i = 0; i < (int)pChild->NumberInputPins(); i++)
        {
            // Check if pin already exists. If it does its types must match or we have a conflict
            CRefObj<IRenderGraphPin> spPin = pChild->GetInputPin(i);
            std::string pinName = spPin->GetName();
            CRefObj<IRenderGraphPin> spInputPin = FindInputPin(pinName);
            if (spInputPin && spInputPin->GetDataType() != spPin->GetDataType())
                CT(E_UNEXPECTED); // Conflict in inputs. Might want to rename the pin to something like NodeName.PinName
            std::any empty;
            CreatePin(this, true, pinName, spPin->GetDataType(), empty);
        }
        pChild->SetParent(this);
        m_children.push_back(CRefObj<IRenderGraphNode>(pChild));
    }
    
    CRefObj<IRenderGraphPin> CRenderGraphNode_Group::AddInputPin(std::string pinName, ERenderGraphDataType dataType)
    {
        std::any empty;
        return CreatePin(this, true, pinName, dataType, empty);
    }
}
