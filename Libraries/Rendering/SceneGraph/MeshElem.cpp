//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <atlbase.h>
#include <d3d11_4.h>

module Rendering.SceneGraph.SceneMeshElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Geometry.Mesh.MeshFuncs;

namespace Caustic
{
    bool CSceneMeshElem::RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** ppMaterial)
    {
        uint32 materialID;
        if (m_spMesh->RayIntersect(ray, pIntersection, &materialID))
        {
            *ppMaterial = m_spMesh->GetMaterial(materialID);
            return true;
        }
        return false;
    }
    
    void CSceneMeshElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
        if (pSceneCtx->m_CurrentPass == c_PassShadow && !pSceneCtx->m_inShadowLightGroup)
            return; // We are in the middle of a shadow pass, but we are not under any lights

        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        if (GetFlags() & ESceneElemFlags::RenderableDirty)
        {
            IShader* pShader = m_spShader;
            if (pShader == nullptr)
                pShader = pSceneCtx->m_spCurrentShader;
            if (pShader == nullptr)
                pShader = pRenderer->GetShaderMgr()->FindShader(L"Default");
            m_spRenderMesh = pRenderer->ToRenderMesh(m_spMesh, pShader);
            SetFlags(GetFlags() & ~ESceneElemFlags::RenderableDirty);
        }
        if (GetFlags() & ESceneElemFlags::MaterialDirty)
        {
            pRenderer->ToRenderMaterials(m_spMesh, (m_spShader) ? m_spShader : pSceneCtx->m_spCurrentShader, m_spRenderMesh, pSceneCtx->m_spCurrentMaterial);
            SetFlags(GetFlags() & ~ESceneElemFlags::MaterialDirty);
        }
        DirectX::XMMATRIX xm(
            pSceneCtx->m_Transform.v[0][0], pSceneCtx->m_Transform.v[0][1], pSceneCtx->m_Transform.v[0][2], pSceneCtx->m_Transform.v[0][3],
            pSceneCtx->m_Transform.v[1][0], pSceneCtx->m_Transform.v[1][1], pSceneCtx->m_Transform.v[1][2], pSceneCtx->m_Transform.v[1][3],
            pSceneCtx->m_Transform.v[2][0], pSceneCtx->m_Transform.v[2][1], pSceneCtx->m_Transform.v[2][2], pSceneCtx->m_Transform.v[2][3],
            pSceneCtx->m_Transform.v[3][0], pSceneCtx->m_Transform.v[3][1], pSceneCtx->m_Transform.v[3][2], pSceneCtx->m_Transform.v[3][3]
        );
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext4> spCtx;
        if (!GetName().empty())
        {
            pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext4>(&spCtx);
            spCtx->BeginEventInt(GetName().c_str(), 0);
        }
#endif
        m_spRenderMesh->Render(pRenderer, pRenderCtx, (IRenderMaterial*)nullptr, nullptr, pSceneCtx->m_lights, &xm);
#ifdef _DEBUG
        if (spCtx)
            spCtx->EndEvent();
#endif
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
        CSceneElem::DrawSelected(pRenderer, this, pSceneCtx);
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

    CRefObj<IMesh> CSceneMeshElem::GetMesh()
    {
        return m_spMesh;
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
