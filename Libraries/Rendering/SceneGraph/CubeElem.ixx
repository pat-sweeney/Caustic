//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneCubeElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Geometry.Mesh.MeshFuncs;
import Geometry.Mesh.IMesh;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneMeshElem;
import Base.Math.Matrix;

export namespace Caustic
{
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

            auto smat = Caustic::Matrix4x4::ScalingMatrix(width / 2.0f, height / 2.0f, depth / 2.0f);
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Cube", nullptr);
            auto spBase = CSceneElem::AsJson(nullptr, pParser);
            spObj->AddElement(spBase);
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::CubeElem; }
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
            CSceneElem::DrawSelected(pRenderer, this, pSceneCtx);
        }

        virtual void GetBBox(BBox3* pBBox) override
        {
            Vector3 v(m_width / 2.0f, m_height / 2.0f, m_depth / 2.0f);
            pBBox->minPt = m_center - v;
            pBBox->maxPt = m_center + v;
            return;
        }

        virtual uint32_t GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32_t flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

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
