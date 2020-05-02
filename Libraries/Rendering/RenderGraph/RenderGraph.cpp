//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include "Base\Math\Matrix.h"
#include "IRenderGraphNode.h"
#include "RenderGraph.h"
#include <vector>
#include <string>
#include <any>

namespace Caustic
{
    //**********************************************************************
    // Method: AddChild
    // See <IRenderGraph>
    //**********************************************************************
    void CRenderGraph::AddChild(IRenderGraphNode *pChild)
    {
        m_spRoot->AddChild(pChild);
    }

    //**********************************************************************
    // Method: Render
    // See <IRenderGraph>
    //**********************************************************************
    void CRenderGraph::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx)
    {
        pRenderCtx->SetMostRecentEpoch(0);
        pRenderCtx->IncrementEpoch();
        CRefObj<IRenderGraphPin> spPin = m_spRoot->FindOutputPin("render");
        if (spPin)
            spPin->GetValue(pRenderer, pRenderCtx);
        return;
    }
    
    //**********************************************************************
    // Method: Lock
    // See <IRenderGraph>
    //**********************************************************************
    void CRenderGraph::Lock()
    {
        EnterCriticalSection(&m_cs);
    }
    
    //**********************************************************************
    // Method: Unlock
    // See <IRenderGraph>
    //**********************************************************************
    void CRenderGraph::Unlock()
    {
        LeaveCriticalSection(&m_cs);
    }
}
