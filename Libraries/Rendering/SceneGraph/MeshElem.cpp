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
    CAUSTICAPI CRefObj<ISceneMeshElem> CreateMeshElem()
    {
        return CRefObj<ISceneMeshElem>(new CSceneMeshElem());
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
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        CRefObj<IGraphics> spGraphics = pRenderer->GetGraphics();
        if (GetFlags() & ESceneElemFlags::RenderableDirty)
        {
            m_spRenderMesh = m_spMesh->ToRenderMesh(pRenderer, pSceneCtx->m_spCurrentShader);
            SetFlags(GetFlags() & ~ESceneElemFlags::RenderableDirty);
        }
        if (GetFlags() & ESceneElemFlags::MaterialDirty)
        {
            m_spMesh->ToRenderMaterials(pRenderer, pSceneCtx->m_spCurrentShader, m_spRenderMesh);
            SetFlags(GetFlags() & ~ESceneElemFlags::MaterialDirty);
        }
        DirectX::XMMATRIX xm(
            pSceneCtx->m_Transform.v[0][0], pSceneCtx->m_Transform.v[0][1], pSceneCtx->m_Transform.v[0][2], pSceneCtx->m_Transform.v[0][3],
            pSceneCtx->m_Transform.v[1][0], pSceneCtx->m_Transform.v[1][1], pSceneCtx->m_Transform.v[1][2], pSceneCtx->m_Transform.v[1][3],
            pSceneCtx->m_Transform.v[2][0], pSceneCtx->m_Transform.v[2][1], pSceneCtx->m_Transform.v[2][2], pSceneCtx->m_Transform.v[2][3],
            pSceneCtx->m_Transform.v[3][0], pSceneCtx->m_Transform.v[3][1], pSceneCtx->m_Transform.v[3][2], pSceneCtx->m_Transform.v[3][3]
        );
        m_spRenderMesh->Render(pRenderer, (IRenderMaterial*)nullptr, nullptr, pSceneCtx->m_lights, &xm);
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
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
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty | ESceneElemFlags::RenderableDirty | ESceneElemFlags::MaterialDirty);
    }

    void CSceneMeshElem::Store(IStream *pStream)
    {
        m_spMesh->Store(pStream);
    }

    void CSceneMeshElem::Load(IStream *pStream)
    {
        m_spMesh = CreateEmptyMesh();
        m_spMesh->Load(pStream);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty | ESceneElemFlags::MaterialDirty);
    }
};
