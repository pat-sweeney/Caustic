//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "SceneGraph.h"
#include "SceneFactory.h"

namespace Caustic
{
    CAUSTICAPI CRefObj<ISceneGraph> CreateSceneGraph()
    {
		CRefObj<ISceneGroupElem> spGroup = CSceneFactory::Instance()->CreateGroupElem();
		return CRefObj<ISceneGraph>(new CSceneGraph(spGroup));
    }

    void CSceneGraph::Lock()
    {
        EnterCriticalSection(&m_cs);
        m_locked = true;
    }

    void CSceneGraph::Unlock()
    {
        m_locked = false;
        LeaveCriticalSection(&m_cs);
    }

    uint32 CSceneGraph::NumberChildren()
    {
        return m_spRoot->NumberChildren();
    }

    CRefObj<ISceneElem> CSceneGraph::GetChild(uint32 index)
    {
        return m_spRoot->GetChild(index);
    }

    void CSceneGraph::Merge(ISceneGraph *pSceneGraph)
    {
        CSceneGraph *pGraph = (CSceneGraph*)pSceneGraph;
        AddChild(pGraph->m_spRoot);
        pGraph->m_spRoot = nullptr;
    }

    void CSceneGraph::AddChild(ISceneElem *pElem)
    {
        for (auto pElem : m_Selected)
            pElem->SetFlags(pElem->GetFlags() & ~ESceneElemFlags::Selected);
        m_Selected.clear();
        m_spRoot->AddChild(pElem);
        m_Selected.push_back(pElem);
        pElem->SetFlags(pElem->GetFlags() | ESceneElemFlags::Selected);
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty);
    }

    void CSceneGraph::InsertChild(ISceneElem *pElem, uint32 index)
    {
        return m_spRoot->InsertChild(pElem, index);
    }

    void CSceneGraph::SetTransform(Matrix4x4 &mat)
    {
        return m_spRoot->SetTransform(mat);
    }

    void CSceneGraph::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        Lock();
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
            {
                Unlock();
                return;
            }
        m_spRoot->Render(pRenderer, pRenderCtx, pSceneCtx);
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
        Unlock();
    }

    void CSceneGraph::Store(IStream *pStream)
    {
        m_spRoot->Store(pStream);
    }

    void CSceneGraph::Load(IStream *pStream)
    {
        m_spRoot->Load(pStream);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }
};
