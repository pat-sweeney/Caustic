//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <any>
#include <d3d11.h>

module Rendering.RenderGraph.RenderGraph;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Matrix;
import Rendering.Caustic.IRenderCtx;
import Rendering.RenderGraph.IRenderGraphNode;

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
