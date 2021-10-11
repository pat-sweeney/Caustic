//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include "ISceneGraph.h"
#include <d3d11.h>
#include <atlbase.h>

namespace Caustic
{
    CAUSTICAPI CRefObj<ISceneGroupElem> CreateGroupElem();
    CAUSTICAPI CRefObj<ISceneMeshElem> CreateMeshElem();
    
    //**********************************************************************
    // Class: CSceneCylinderElem
    // Defines a Cylinder element in our scene graph
    //**********************************************************************
    class CSceneCylinderElem :
        public CSceneElem,
        public ISceneCylinderElem,
        public CRefCount
    {
        CRefObj<ISceneGroupElem> m_spXformElem;
        CRefObj<ISceneMeshElem> m_spMeshElem;
        CRefObj<IMesh> m_spMesh;
        Vector3 m_center;
        float m_height;
        float m_topRadius;
        float m_bottomRadius;
    public:
        //**********************************************************************
        // Constructor: CSceneCylinderElem
        // Default constructor
        //**********************************************************************
        CSceneCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
        {
            m_spXformElem = CreateGroupElem();
            m_spMeshElem = CreateMeshElem();
            m_spXformElem->AddChild(m_spMeshElem);
            SetPosition(center, height, topRadius, bottomRadius);
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::LineElem; }
        virtual std::wstring GetName() override { return CSceneElem::GetName(); }
        virtual void SetName(const wchar_t* name) override { return CSceneElem::SetName(name); }
        virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) override
        {
            CSceneElem::SetPreRenderCallback(prerenderCallback);
        }
        virtual void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback) override
        {
            CSceneElem::SetPostRenderCallback(postrenderCallback);
        }
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override
        {
            if (!(m_passes & pRenderCtx->GetCurrentPass()))
                return;
            if (m_prerenderCallback)
                if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                    return;
            m_spXformElem->Render(pRenderer, pRenderCtx, pSceneCtx);
            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }
        virtual void GetBBox(BBox3* pBBox) override { return; }
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { return; }
        virtual void Store(IStream* pStream) override { return; }

        //**********************************************************************
        // ISceneCylinderElem
        //**********************************************************************
        virtual void SetPosition(Vector3& center, float height, float topRadius, float bottomRadius) override
        {
            m_center = center;
            m_topRadius = topRadius;
            m_bottomRadius = bottomRadius;
            std::vector<Vector3> pts;
            const int subdivisions = 32;
            float dy = 0.0f;
            float radius = bottomRadius;
            for (int i = 0; i < subdivisions; i++)
            {
                Vector3 v;
                v.x = radius;
                v.y = dy;
                v.z = 0.0f;
                pts.push_back(v);
                dy += height / float(subdivisions);
                radius += (topRadius - bottomRadius) / float(subdivisions);
            }
            m_spMesh = Caustic::CreateSurfaceRevolution(pts, subdivisions, subdivisions, 360.0f);
            m_spMeshElem->SetMesh(m_spMesh);
            auto tmat = Caustic::Matrix4x4::TranslationMatrix(center.x, center.y, center.z);
            m_spXformElem->SetTransform(tmat);
        }

        virtual void GetPosition(Vector3* pCenter, float* pHeight, float* pTopRadius, float* pBottomRadius) override
        {
            *pCenter = m_center;
            *pHeight = m_height;
            *pTopRadius = m_topRadius;
            *pBottomRadius = m_bottomRadius;
        }
    };
};
