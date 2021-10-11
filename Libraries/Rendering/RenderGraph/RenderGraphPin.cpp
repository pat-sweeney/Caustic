//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\error.h"
#include "Rendering\Caustic\Caustic.h"
#include "RenderGraphPin.h"
#include "RenderGraphNode.h"
#include "RenderGraphDiagnostics.h"
#include <vector>
#include <string>
#include <any>
#include <map>

namespace Caustic
{
    std::any CRenderGraphPin::GetValue(IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        IRenderGraphPin* pPin = this;
        if (m_isInput)
        {
            if (pPin->GetConnection(0) == nullptr)
            {
                // Pin is not attached to anything
                // Ask parent for its value
                CRefObj<IRenderGraphNode> spPinParent = pPin->GetParent(); // This pin's graph node
                CRefObj<IRenderGraphNode> spOuterParent = spPinParent->GetParent(); // Pin's grandparent node
                if (spOuterParent)
                {
                    CRefObj<IRenderGraphPin> spOuterPin = spOuterParent->FindInputPin(pPin->GetName());
                    if (spOuterPin)
                    {
                        std::any parentVal = spOuterPin->GetValue(pRenderer, pRenderCtx);
                        if (parentVal.has_value())
                            return parentVal;
                    }
                }

                // So parent doesn't have a value to use, so use our default value
                return pPin->GetDefaultValue();
            }
            else
                return pPin->GetConnection(0)->GetValue(pRenderer, pRenderCtx);
        }
        return pPin->GetParent()->GetPinValue(pPin, pRenderer, pRenderCtx);
    }

    void CRenderGraphPin::LinkTo(IRenderGraphPin* pPin)
    {
        if (m_type != pPin->GetDataType())
        {
            SpewWarning(L"Attempting to connect pin '%S' on node '%S' to pin '%S' on node '%S'. Pin data types must match.", GetName(), GetParent()->GetName(), pPin->GetName(), pPin->GetParent()->GetName());
            CT(E_UNEXPECTED);
            return;
        }

        if (this == pPin)
        {
            SpewWarning(L"Attempting to connect pin '%S' on node '%S' to itself. Request ignored.", pPin->GetName(), pPin->GetParent()->GetName());
            return;
        }

        if (m_isInput)
        {
            // Check if input pin is already connected to something else. If so, disconnect it
            if (NumberConnections() != 0)
            {
                SpewWarning(L"Input pin '%S' on node '%S' already connected. Disconnecting.", GetName(), GetParent()->GetName());
                ClearLink(0);
            }
        }
        AddLink(pPin);
        pPin->AddLink(this);
    }
    
    void CRenderGraphPin::ClearLink(int index)
    {
        m_connections.erase(m_connections.begin() + index);
    }

    void CRenderGraphPin::AddLink(IRenderGraphPin *pPin)
    {
        m_connections.push_back(pPin);
    }
}
