//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\Material.h"
#include "MaterialElem.h"
#include <string>

namespace Caustic
{
    namespace Scene
    {
        CAUSTICAPI void CreateMaterialElem(ISceneMaterialElem **ppElem)
        {
            std::unique_ptr<CSceneMaterialElem> spMeshObj(new CSceneMaterialElem());
            Caustic::CreateMaterial(&spMeshObj->m_spMaterial);
            *ppElem = spMeshObj.release();
            (*ppElem)->AddRef();
        }
    };

    std::wstring &CSceneMaterialElem::Name()
    {
        return CSceneElem::m_Name;
    }

    void CSceneMaterialElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        CRefObj<IMaterialAttrib> spOldMaterial = pSceneCtx->m_spCurrentMaterial;
        pSceneCtx->m_spCurrentMaterial = m_spMaterial;
        CSceneGroupElem::Render(pRenderer, pRenderCtx, pSceneCtx);
        pSceneCtx->m_spCurrentMaterial = spOldMaterial;
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
