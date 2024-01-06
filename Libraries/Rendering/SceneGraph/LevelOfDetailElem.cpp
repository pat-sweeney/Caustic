//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <string>
#include <functional>
#include <d3d11_4.h>

module Rendering.SceneGraph.SceneLevelOfDetailElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ICamera;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneLevelOfDetailElem;

namespace Caustic
{
    CRefObj<IJSonObj> CSceneLevelOfDetailElem::AsJson(const char* pPropertyName, IJSonParser* pParser)
    {
        auto spGroup = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "LOD", nullptr);
        auto spBase = CSceneElem::AsJson(nullptr, pParser);
        spGroup->AddElement(spBase);
        auto spLevels = pParser->CreateJSonArray("Levels", nullptr);
        spGroup->AddElement(spLevels);
        for (size_t i = 0; i < m_children.size(); i++)
        {
            auto spLOD = pParser->CreateJSonMap(nullptr, nullptr);
            spLOD->AddElement(m_children[i].m_spChild->AsJson("Child", pParser));
            spLOD->AddElement(pParser->CreateJSon("MinRenderDistance", (int)m_children[i].m_minRenderDistance));
            spLOD->AddElement(pParser->CreateJSon("MaxRenderDistance", (int)m_children[i].m_maxRenderDistance));
            spLevels->AddElement(spLOD);
        }
        return spGroup;
    }

    bool CSceneLevelOfDetailElem::RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** ppMaterial)
    {
        bool found = false;
        for (size_t i = 0; i < m_children.size(); i++)
        {
            RayIntersect3 ri;
            if (m_children[i].m_spChild->RayIntersect(ray, &ri, ppMaterial))
            {
                if (ri.hitTime < pIntersection->hitTime)
                {
                    found = true;
                    *pIntersection = ri;
                }
            }
        }
        return found;
    }
    
    void CSceneLevelOfDetailElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext4> spCtx;
        if (!GetName().empty())
        {
            pRenderer->GetContext()->QueryInterface<ID3D11DeviceContext4>(&spCtx);
            spCtx->BeginEventInt(GetName().c_str(), 0);
        }
#endif
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        Vector3 pos;
        pRenderer->GetCamera()->GetPosition(&pos, nullptr, nullptr, nullptr, nullptr, nullptr);
        for (size_t i = 0; i < m_children.size(); i++)
        {
            if (m_children[i].m_spChild->GetFlags() & ESceneElemFlags::Hidden)
                continue;
            BBox3 bbox;
            m_children[i].m_spChild->GetBBox(&bbox);
            Vector3 center = bbox.Center();
            float dist = (pos - center).Length();
            if (dist >= m_children[i].m_minRenderDistance && dist <= m_children[i].m_maxRenderDistance)
            {
                m_children[i].m_spChild->Render(pRenderer, pRenderCtx, pSceneCtx);
            }
        }
        DrawSelected(pRenderer, this, pSceneCtx);
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
#ifdef _DEBUG
        if (!GetName().empty())
            spCtx->EndEvent();
#endif
    }

    void CSceneLevelOfDetailElem::GetBBox(BBox3* pBBox)
    {
        if (GetFlags() & ESceneElemFlags::BBoxDirty)
        {
            for (size_t i = 0; i < m_children.size(); i++)
            {
                BBox3 bb;
                m_children[i].m_spChild->GetBBox(&bb);
                m_BBox.AddPoint(bb.minPt);
                m_BBox.AddPoint(bb.maxPt);
            }
            SetFlags(GetFlags() & ~ESceneElemFlags::BBoxDirty);
        }
        *pBBox = m_BBox;
    }

    void CSceneLevelOfDetailElem::AddChild(ISceneElem* pElem, float minRenderDistance, float maxRenderDistance)
    {
        LODChild child;
        child.m_spChild = pElem;
        child.m_minRenderDistance = minRenderDistance;
        child.m_maxRenderDistance = maxRenderDistance;
        m_children.push_back(child);
    }

    void CSceneLevelOfDetailElem::Store(IStream* pStream)
    {
        ULONG bytesWritten;
        uint32_t numChildren = (uint32_t)m_children.size();
        CT(pStream->Write(&numChildren, sizeof(numChildren), &bytesWritten));
        for (uint32_t i = 0; i < (uint32_t)numChildren; i++)
        {
            CT(pStream->Write(&m_children[i].m_minRenderDistance, sizeof(m_children[i].m_minRenderDistance), &bytesWritten));
            CT(pStream->Write(&m_children[i].m_maxRenderDistance, sizeof(m_children[i].m_maxRenderDistance), &bytesWritten));
            uint32_t type = (uint32_t)m_children[i].m_spChild->GetType();
            CT(pStream->Write(&type, sizeof(uint32_t), &bytesWritten));
            m_children[i].m_spChild->Store(pStream);
        }
    }

    void CSceneLevelOfDetailElem::Load(IStream* pStream)
    {
        CRefObj<ISceneFactory> spSceneFactory = CSceneFactory::Instance();
        ULONG bytesRead;
        uint32_t numChildren;
        CT(pStream->Read(&numChildren, sizeof(numChildren), &bytesRead));
        for (uint32_t i = 0; i < (uint32_t)numChildren; i++)
        {
            CSceneLevelOfDetailElem::LODChild child;
            CT(pStream->Read(&child.m_minRenderDistance, sizeof(child.m_minRenderDistance), &bytesRead));
            CT(pStream->Read(&child.m_maxRenderDistance, sizeof(child.m_maxRenderDistance), &bytesRead));
            CRefObj<ISceneElem> spElem;
            uint32_t type;
            CT(pStream->Read(&type, sizeof(uint32_t), &bytesRead));
            switch ((ESceneElemType)type)
            {
            case ESceneElemType::Group:
            {
                CRefObj<ISceneGroupElem> spGroupElem = spSceneFactory->CreateGroupElem();
                spElem = spGroupElem;
            }
            break;
            case ESceneElemType::Material:
            {
                CRefObj<ISceneMaterialElem> spMaterialElem = spSceneFactory->CreateMaterialElem();
                spElem = spMaterialElem;
            }
            break;
            case ESceneElemType::Mesh:
            {
                CRefObj<ISceneMeshElem> spMeshElem = spSceneFactory->CreateMeshElem();
                spElem = spMeshElem;
            }
            break;
            default:
                _ASSERTE(FALSE); // Unknown element type
                break;
            }
            child.m_spChild = spElem;
            spElem->Load(pStream);
            m_children.push_back(child);
        }
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }
}
