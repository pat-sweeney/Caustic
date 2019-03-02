//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include "SceneGraphImpl.h"
#include "SceneFactory.h"
#include "GroupElem.h"
#include <string>
#include <functional>

namespace Caustic
{
    CAUSTICAPI void CreateGroupElem(ISceneGroupElem **ppElem)
    {
        std::unique_ptr<CSceneGroupElem> spGroupObj(new CSceneGroupElem());
        *ppElem = spGroupObj.release();
        (*ppElem)->AddRef();
    }

    std::wstring &CSceneGroupElem::Name()
    {
        return m_Name;
    }

    void CSceneGroupElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
    {
        Matrix4x4 old = pSceneCtx->m_Transform;
        pSceneCtx->m_Transform = pSceneCtx->m_Transform * m_Transform;

        std::vector<CRefObj<IPointLight>> lights = pSceneCtx->m_lights;
        for (size_t i = 0; i < m_Children.size(); i++)
        {
            if (m_Children[i]->GetType() == ESceneElemType::PointLight)
            {
                m_Children[i]->Render(pRenderer, pRenderCtx, pSceneCtx);
            }
        }
        for (size_t i = 0; i < m_Children.size(); i++)
        {
            if (m_Children[i]->GetType() != ESceneElemType::PointLight)
            {
                m_Children[i]->Render(pRenderer, pRenderCtx, pSceneCtx);
            }
        }
        pSceneCtx->m_lights = lights;
        DrawSelected(pRenderer, this, pSceneCtx);
        pSceneCtx->m_Transform = old;
    }

    void CSceneGroupElem::GetBBox(BBox3 *pBBox)
    {
        if (GetFlags() & ESceneElemFlags::BBoxDirty)
        {
            for (size_t i = 0; i < m_Children.size(); i++)
            {
                BBox3 bb;
                m_Children[i]->GetBBox(&bb);
                m_BBox.AddPoint(bb.minPt);
                m_BBox.AddPoint(bb.maxPt);
            }
            SetFlags(GetFlags() & ~ESceneElemFlags::BBoxDirty);
        }
        *pBBox = m_BBox;
    }
    
    uint32 CSceneGroupElem::NumberChildren()
    {
        return static_cast<uint32>(m_Children.size());
    }

    CRefObj<ISceneElem> CSceneGroupElem::GetChild(uint32 index)
    {
        return m_Children[index];
    }

    void CSceneGroupElem::AddChild(ISceneElem *pElem)
    {
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty);
        m_Children.push_back(pElem);
    }

    void CSceneGroupElem::InsertChild(ISceneElem *pElem, uint32 index)
    {
        SetFlags(GetFlags() | ESceneElemFlags::BBoxDirty);
        m_Children.insert(m_Children.begin() + index, pElem);
    }

    void CSceneGroupElem::Store(IStream *pStream, std::function<void(ISceneElem *pElem)> func)
    {
        ULONG bytesWritten;
        uint32 numChildren = (uint32)m_Children.size();
        CT(pStream->Write(&numChildren, sizeof(numChildren), &bytesWritten));
        CT(pStream->Write(&m_Transform, sizeof(m_Transform), &bytesWritten));
        for (uint32 i = 0; i < (uint32)numChildren; i++)
        {
            uint32 type = (uint32)m_Children[i]->GetType();
            CT(pStream->Write(&type, sizeof(uint32), &bytesWritten));
            func(m_Children[i].p);
        }
    }

    void CSceneGroupElem::Load(IStream *pStream, std::function<void(ISceneElem *pElem)> func)
    {
        ULONG bytesRead;
        uint32 numChildren;
        CT(pStream->Read(&numChildren, sizeof(numChildren), &bytesRead));
        CT(pStream->Read(&m_Transform, sizeof(m_Transform), &bytesRead));
        for (uint32 i = 0; i < (uint32)numChildren; i++)
        {
            CRefObj<ISceneElem> spElem;
            uint32 type;
            CT(pStream->Read(&type, sizeof(uint32), &bytesRead));
            switch ((ESceneElemType)type)
            {
            case ESceneElemType::Group:
            {
                CRefObj<ISceneGroupElem> spGroupElem;
				Caustic::CSceneFactory::Instance()->CreateGroupElem(&spGroupElem);
                spElem = spGroupElem.p;
            }
            break;
            case ESceneElemType::Material:
            {
                CRefObj<ISceneMaterialElem> spMaterialElem;
				Caustic::CSceneFactory::Instance()->CreateMaterialElem(&spMaterialElem);
                spElem = spMaterialElem.p;
            }
            break;
            case ESceneElemType::Mesh:
            {
                CRefObj<ISceneMeshElem> spMeshElem;
                Caustic::CSceneFactory::Instance()->CreateMeshElem(&spMeshElem);
                spElem = spMeshElem.p;
            }
            break;
            case ESceneElemType::PointLight:
            {
                CRefObj<IScenePointLightElem> spPointLightElem;
				Caustic::CSceneFactory::Instance()->CreatePointLightElem(&spPointLightElem);
                spElem = spPointLightElem.p;
            }
            break;
            default:
                _ASSERTE(FALSE); // Unknown element type
                break;
            }
            func(spElem.p);
            m_Children.push_back(spElem.p);
        }
        SetFlags(GetFlags() | ESceneElemFlags::RenderableDirty | ESceneElemFlags::BBoxDirty);
    }

    void CSceneGroupElem::Store(IStream *pStream)
    {
        Store(pStream, [this, pStream](ISceneElem *pElem) { pElem->Store(pStream); });
    }

    void CSceneGroupElem::Load(IStream *pStream)
    {
        Load(pStream, [this, pStream](ISceneElem *pElem) { pElem->Load(pStream); });
    }
}
