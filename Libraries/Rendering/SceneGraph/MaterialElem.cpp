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
    CAUSTICAPI CRefObj<ISceneMaterialElem> CreateMaterialElem()
    {
        std::unique_ptr<CSceneMaterialElem> spMaterialElem(new CSceneMaterialElem());
		CRefObj<IMaterialAttrib> spMaterial = Caustic::CreateMaterialAttrib();
        spMaterialElem->SetMaterial(spMaterial);
        return CRefObj<ISceneMaterialElem>(spMaterialElem.release());
    }

    std::wstring &CSceneMaterialElem::Name()
    {
        return CSceneElem::m_Name;
    }

    void CSceneMaterialElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        CRefObj<IMaterialAttrib> spOldMaterial = pSceneCtx->m_spCurrentMaterial;
        CRefObj<IShader> spOldShader = pSceneCtx->m_spCurrentShader;
        pSceneCtx->m_spCurrentMaterial = m_spMaterial;
        pSceneCtx->m_spCurrentShader = m_spShader;
        CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        pSceneCtx->m_spCurrentMaterial = spOldMaterial;
        pSceneCtx->m_spCurrentShader = spOldShader;
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
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
