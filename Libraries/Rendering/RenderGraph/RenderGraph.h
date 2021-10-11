//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Ray.h"
#include "Rendering\Caustic\Caustic.h"
#include "IRenderGraph.h"
#include "IRenderGraphNode.h"
#include "IRGNGroup.h"
#include "RenderGraphFactory.h"
#include <vector>
#include <string>
#include <any>
#include <map>

namespace Caustic
{
    //**********************************************************************
    // Class: CRenderGraph
    // Implements <IRenderGraph>
    //
    // Members:
    // m_nodes - list of nodes in the graph
    // m_locked - is the graph currently locked for updates
    // m_cs - critical section used by lock
    // m_spRenderer - our renderer
    // m_Flags - miscellaneous flags
    //**********************************************************************
    class CRenderGraph : public IRenderGraph, public CRefCount
    {
        CRefObj<IRenderGraphFactory> m_spRGFactory;
        CRefObj<IRenderGraphNode_Group> m_spRoot;
        bool m_locked;
        CRITICAL_SECTION m_cs;
        CRefObj<IRenderer> m_spRenderer;
        uint32 m_Flags;
    public:
        CRenderGraph() :
            m_locked(false)
        {
            InitializeCriticalSection(&m_cs);
            m_spRGFactory = CreateRenderGraphFactory();
            m_spRoot = m_spRGFactory->CreateGroupNode();
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderGraph
        //**********************************************************************
        virtual void AddChild(IRenderGraphNode* pChild) override;
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx) override;
        virtual void Lock() override;
        virtual void Unlock() override;
    };
}
