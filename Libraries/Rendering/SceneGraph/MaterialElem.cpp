//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\MaterialAttrib.h"
#include "MaterialElem.h"
#include <string>

namespace Caustic
{
    CAUSTICAPI void CreateMaterialElem(ISceneMaterialElem **ppElem)
    {
        std::unique_ptr<CSceneMaterialElem> spMeshObj(new CSceneMaterialElem());
		CRefObj<IMaterialAttrib> spMaterial;
        Caustic::CreateMaterial(&spMaterial);
		spMeshObj->SetMaterial(spMaterial);
        *ppElem = spMeshObj.release();
        (*ppElem)->AddRef();
    }

    std::wstring &CSceneMaterialElem::Name()
    {
        return CSceneElem::m_Name;
    }

    void CSceneMaterialElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        CRefObj<IMaterialAttrib> spOldMaterial = pSceneCtx->m_spCurrentMaterial;
        CRefObj<IShader> spOldShader = pSceneCtx->m_spCurrentShader;
        pSceneCtx->m_spCurrentMaterial = m_spMaterial;
        pSceneCtx->m_spCurrentShader = m_spShader;
        CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        pSceneCtx->m_spCurrentMaterial = spOldMaterial;
        pSceneCtx->m_spCurrentShader = spOldShader;
    }

    void CSceneMaterialElem::Store(IStream *pStream)
    {
        m_spMaterial->Store(pStream);
    }

    void CSceneMaterialElem::Load(IStream *pStream)
    {
        m_spMaterial->Load(pStream);
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    };
}
