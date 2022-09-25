//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <string>
#include <functional>
#include <d3d11_4.h>

module Rendering.SceneGraph.SceneGroupElem;
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
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneGroupElem;

namespace Caustic
{
    CRefObj<ISceneGroupElem> CreateGroupElem()
    {
        return CRefObj<ISceneGroupElem>(new CSceneGroupElem());
    }

    void CSceneGroupElem::Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx)
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
        Matrix4x4 old = pSceneCtx->m_Transform;
        pSceneCtx->m_Transform = pSceneCtx->m_Transform * m_Transform;
        for (size_t i = 0; i < m_Children.size(); i++)
        {
            if (m_Children[i]->GetFlags() & ESceneElemFlags::Hidden)
                continue;
            m_Children[i]->Render(pRenderer, pRenderCtx, pSceneCtx);
        }
        pSceneCtx->m_Transform = old;
        DrawSelected(pRenderer, this, pSceneCtx);
        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());
#ifdef _DEBUG
        if (!GetName().empty())
            spCtx->EndEvent();
#endif
    }

    void CSceneGroupElem::GetBBox(BBox3 *pBBox)
    {
        if (GetFlags() & ESceneElemFlags::BBoxDirty)
        {
            for (size_t i = 0; i < m_Children.size(); i++)
            {
                BBox3 bb;
                m_Children[i]->GetBBox(&bb);
                Vector4 corners[8]{
                    Vector4(bb.minPt.x, bb.minPt.y, bb.minPt.z, 1.0f),
                    Vector4(bb.minPt.x, bb.minPt.y, bb.maxPt.z, 1.0f),
                    Vector4(bb.minPt.x, bb.maxPt.y, bb.minPt.z, 1.0f),
                    Vector4(bb.minPt.x, bb.maxPt.y, bb.maxPt.z, 1.0f),
                    Vector4(bb.maxPt.x, bb.minPt.y, bb.minPt.z, 1.0f),
                    Vector4(bb.maxPt.x, bb.minPt.y, bb.maxPt.z, 1.0f),
                    Vector4(bb.maxPt.x, bb.maxPt.y, bb.minPt.z, 1.0f),
                    Vector4(bb.maxPt.x, bb.maxPt.y, bb.maxPt.z, 1.0f),
                };
                for (int i = 0; i < 8; i++)
                {
                    Vector4 np = corners[i] * m_Transform;
                    Vector3 v(np.x, np.y, np.z);
                    m_BBox.AddPoint(v);
                }
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
            func(m_Children[i]);
        }
    }

    void CSceneGroupElem::Load(IStream *pStream, std::function<void(ISceneElem *pElem)> func)
    {
        CRefObj<ISceneFactory> spSceneFactory = CSceneFactory::Instance();
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
            func(spElem);
            m_Children.push_back(spElem);
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
