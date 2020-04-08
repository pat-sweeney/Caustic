//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "MeshElem.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI void CreateMeshElem(ISceneMeshElem **ppElem)
    {
        std::unique_ptr<CSceneMeshElem> spMeshObj(new CSceneMeshElem());
        *ppElem = spMeshObj.release();
        (*ppElem)->AddRef();
    }

    std::wstring &CSceneMeshElem::Name()
    {
        return CSceneElem::m_Name;
    }

    void CSceneElem::DrawSelected(IRenderer *pRenderer, ISceneElem *pElem, SceneCtx *pSceneCtx)
    {
        // Draw manipulator around currently selected object
        if (pElem->GetFlags() & (uint32)ESceneElemFlags::Selected)
        {
            BBox3 bbox;
            pElem->GetBBox(&bbox);
            Vector4 minPt = bbox.minPt * pSceneCtx->m_Transform;
            Vector4 maxPt = bbox.maxPt * pSceneCtx->m_Transform;
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, minPt.y, minPt.z),
                Vector3(minPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(minPt.x, maxPt.y, minPt.z),
                Vector3(minPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, minPt.y, minPt.z),
                Vector3(maxPt.x, minPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
            pRenderer->DrawLine(
                Vector3(maxPt.x, maxPt.y, minPt.z),
                Vector3(maxPt.x, maxPt.y, maxPt.z),
                Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }

    void CSceneMeshElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        CRefObj<IGraphics> spGraphics;
        pRenderer->GetGraphics(&spGraphics);
        if (GetFlags() & ESceneElemFlags::RenderableDirty)
        {
            m_spMesh->ToRenderMesh(pRenderer, pSceneCtx->m_spCurrentShader, &m_spRenderMesh);
            SetFlags(GetFlags() & ~ESceneElemFlags::RenderableDirty);
        }
        m_spRenderMesh->Render(pRenderer, pSceneCtx->m_lights);
    }

    void CSceneMeshElem::GetBBox(BBox3 *pBBox)
    {
        if (GetFlags() & ESceneElemFlags::BBoxDirty)
        {
            if (m_spMesh)
                m_spMesh->GetBBox(&m_BBox);
            SetFlags(GetFlags() & ~ESceneElemFlags::BBoxDirty);
        }
        *pBBox = m_BBox;
    }
    
    void CSceneMeshElem::SetMesh(IMesh *pMesh)
    {
        m_spMesh = pMesh;
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty | ESceneElemFlags::RenderableDirty);
    }

    void CSceneMeshElem::Store(IStream *pStream)
    {
        m_spMesh->Store(pStream);
    }

    void CSceneMeshElem::Load(IStream *pStream)
    {
        CreateEmptyMesh(&m_spMesh);
        m_spMesh->Load(pStream);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }
};
