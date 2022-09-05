//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneSphereElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.MeshFuncs;
import Geometry.Mesh.IMesh;
import Parsers.JSon.IJSonParser;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneSphereElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneMeshElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneSphereElem
    // Defines a sphere element in our scene graph
    //**********************************************************************
    class CSceneSphereElem :
        public CSceneElem,
        public ISceneSphereElem,
        public CRefCount
    {
        CRefObj<ISceneGroupElem> m_spXformElem;
        CRefObj<ISceneMeshElem> m_spMeshElem;
        CRefObj<IMesh> m_spMesh;
        Vector3 m_center;
        float m_radius;
    public:
        //**********************************************************************
        // Constructor: CSceneSphereElem
        // Default constructor
        //**********************************************************************
        CSceneSphereElem(Vector3& center, float radius)
        {
            m_spXformElem = CreateGroupElem();

            auto smat = Caustic::Matrix4x4::ScalingMatrix(radius, radius, radius);
            auto tmat = Caustic::Matrix4x4::TranslationMatrix(center.x, center.y, center.z);
            auto mat = smat * tmat;
            m_spXformElem->SetTransform(mat);

            m_spMeshElem = CreateMeshElem();
            m_spXformElem->AddChild(m_spMeshElem);
            m_spMesh = Caustic::CreateSphere(32);
            m_spMeshElem->SetMesh(m_spMesh);
            m_center = center;
            m_radius = radius;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char *pPropertyName, IJSonParser* pParser) override
        {
            return pParser->CreateJSonMap(pPropertyName,
                pParser->CreateJSon(    "Type", "SphereElem"),
                m_spXformElem->AsJson(  "Xform", pParser),
                pParser->CreateJSon(    "Mesh", (void*)&m_spMesh.p),
                m_spMeshElem->AsJson(   "MeshElem", pParser),
                pParser->CreateJSonMap( "Center",
                            pParser->CreateJSon("Type", "Vector3"),
                            pParser->CreateJSon("Addr", (void*)&m_center),
                            pParser->CreateJSon("x", m_center.x),
                            pParser->CreateJSon("y", m_center.y),
                            pParser->CreateJSon("z", m_center.z)),
                pParser->CreateJSonMap( "Radius",
                            pParser->CreateJSon("Type", "Float"),
                            pParser->CreateJSon("Addr", (void*)&m_radius),
                            pParser->CreateJSon("Value", m_radius)));
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::SphereElem; }
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
        virtual void GetBBox(BBox3* pBBox) override
        {
            pBBox->minPt = m_center - m_radius;
            pBBox->maxPt = m_center + m_radius;
            return;
        }
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
        // ISceneSphereElem
        //**********************************************************************
        virtual void SetPosition(Vector3& center, float& radius) override
        {
            auto smat = Caustic::Matrix4x4::ScalingMatrix(radius, radius, radius);
            auto tmat = Caustic::Matrix4x4::TranslationMatrix(center.x, center.y, center.z);
            auto mat = smat * tmat;
            m_spXformElem->SetTransform(mat);
            m_center = center;
            m_radius = radius;
        }

        virtual void GetPosition(Vector3* pCenter, float* pRadius) override
        {
            *pCenter = m_center;
            *pRadius = m_radius;
        }
    };
};
