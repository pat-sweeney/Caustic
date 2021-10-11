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
    // Class: CSceneCubeElem
    // Defines a cube element in our scene graph
    //**********************************************************************
    class CSceneCubeElem :
        public CSceneElem,
        public ISceneCubeElem,
        public CRefCount
    {
        CRefObj<ISceneGroupElem> m_spXformElem;
        CRefObj<ISceneMeshElem> m_spMeshElem;
        CRefObj<IMesh> m_spMesh;
        Vector3 m_center;
        float m_width;
        float m_height;
        float m_depth;
    public:
        //**********************************************************************
        // Constructor: CSceneSphereElem
        // Default constructor
        //**********************************************************************
        CSceneCubeElem(Vector3 &center, float width, float height, float depth)
        {
            m_center = center;
            m_width = width;
            m_height = height;
            m_depth = depth;
            m_spXformElem = CreateGroupElem();

            auto smat = Caustic::Matrix4x4::ScalingMatrix(width, height, depth);
            auto tmat = Caustic::Matrix4x4::TranslationMatrix(center.x, center.y, center.z);
            auto mat = smat * tmat;
            m_spXformElem->SetTransform(mat);

            m_spMeshElem = CreateMeshElem();
            m_spXformElem->AddChild(m_spMeshElem);
            m_spMesh = Caustic::CreateCube();
            m_spMeshElem->SetMesh(m_spMesh);
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
        // ISceneCubeElem
        //**********************************************************************
        virtual void SetPosition(Vector3& center, float width, float height, float depth) override
        {
            m_center = center;
            m_width = width;
            m_height = height;
            m_depth = depth;
            auto smat = Caustic::Matrix4x4::ScalingMatrix(width, height, depth);
            auto tmat = Caustic::Matrix4x4::TranslationMatrix(center.x, center.y, center.z);
            auto mat = smat * tmat;
            m_spXformElem->SetTransform(mat);
        }

        virtual void GetPosition(Vector3* pCenter, float* pWidth, float* pHeight, float* pDepth) override
        {
            *pCenter = m_center;
            *pWidth = m_width;
            *pHeight = m_height;
            *pDepth = m_depth;
        }
    };
};
