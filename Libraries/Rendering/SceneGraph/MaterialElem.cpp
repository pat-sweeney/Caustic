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

    void CSceneMaterialElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        if (m_spMaterial->GetIsTransparent() && pSceneCtx->m_CurrentPass != c_PassTransparent)
            return;
        
        // TODO:
        // Need to clean this up. In some ways the MaterialElem in the scene graph
        // is kind of useless since in general the mesh itself has the materials on it
        // already. However, I think it will probably be useful to keep this node as
        // an override material mechanism. I need to give this more thought. For now,
        // I've commented this out until I figure out how I want to handle materials
        // inside the scene graph.

        //CRefObj<IMaterialAttrib> spOldMaterial = pSceneCtx->m_spCurrentMaterial;
        //CRefObj<IShader> spOldShader = pSceneCtx->m_spCurrentShader;
        //pSceneCtx->m_spCurrentMaterial = m_spMaterial;
        //pSceneCtx->m_spCurrentShader = m_spShader;
        CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        //pSceneCtx->m_spCurrentMaterial = spOldMaterial;
        //pSceneCtx->m_spCurrentShader = spOldShader;
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
