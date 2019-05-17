//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "MeshElem.h"
#include "SceneElem.h"
#include <string>

namespace Caustic
{
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
        if (GetFlags() & ESceneElemFlags::RenderableDirty)
        {
            for (uint32 i = 0; i < m_spMesh->NumberSubMeshes(); i++)
            {
#if 0
                CRefObj<ISubMesh> spSubMesh;
                m_spMesh->GetSubMesh(i, &spSubMesh);
                CRefObj<IRenderable> spRenderable;
                Caustic::CCausticFactory::Instance()->CreateRenderable(spSubMesh.p, pSceneCtx->m_spCurrentMaterial.p, pSceneCtx->m_spCurrentPixelShader.p, &spRenderable);
                if (spRenderable != nullptr)
                {
                    m_renderables.push_back(spRenderable);
                    SetFlags(GetFlags() & ~ESceneElemFlags::RenderableDirty);
                }
#endif
            }
        }
        for (uint32 i = 0; i < (uint32)m_renderables.size(); i++)
        {
            if (m_renderables[i]->InPass(pSceneCtx->m_CurrentPass))
            {
                DirectX::XMMATRIX dxmat(
                    pSceneCtx->m_Transform.v[0][0], pSceneCtx->m_Transform.v[0][1], pSceneCtx->m_Transform.v[0][2], pSceneCtx->m_Transform.v[0][3],
                    pSceneCtx->m_Transform.v[1][0], pSceneCtx->m_Transform.v[1][1], pSceneCtx->m_Transform.v[1][2], pSceneCtx->m_Transform.v[1][3],
                    pSceneCtx->m_Transform.v[2][0], pSceneCtx->m_Transform.v[2][1], pSceneCtx->m_Transform.v[2][2], pSceneCtx->m_Transform.v[2][3],
                    pSceneCtx->m_Transform.v[3][0], pSceneCtx->m_Transform.v[3][1], pSceneCtx->m_Transform.v[3][2], pSceneCtx->m_Transform.v[3][3]);
                m_renderables[i]->SetTransform(dxmat);
                m_renderables[i]->Render(pRenderer, pSceneCtx->m_lights, pRenderCtx);
                DrawSelected(pRenderer, this, pSceneCtx);
            }
        }
    }

    void CSceneMeshElem::GetBBox(BBox3 *pBBox)
    {
        if (GetFlags() & ESceneElemFlags::BBoxDirty)
        {
            if (m_spMesh.p)
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
