//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneCylinderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.CausticFactory;
import Geometry.Mesh.MeshFuncs;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneCylinderElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneMeshElem;

export namespace Caustic
{
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Cylinder", nullptr);
            auto spBase = CSceneElem::AsJson("Base", pParser);
            spObj->AddElement(spBase);
            auto spCenter = pParser->CreateJSonMap("Center",
                pParser->CreateJSon("x", m_center.x),
                pParser->CreateJSon("y", m_center.y),
                pParser->CreateJSon("z", m_center.z),
                nullptr);
            spObj->AddElement(spCenter);
            spObj->AddElement(pParser->CreateJSon("Height", m_height));
            spObj->AddElement(pParser->CreateJSon("TopRadius", m_topRadius));
            spObj->AddElement(pParser->CreateJSon("BottomRadius", m_bottomRadius));
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::CylinderElem; }
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
            float r = std::max<float>(m_topRadius, m_bottomRadius);
            Vector3 v(r, m_height / 2.0f, r);
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
        // ISceneCylinderElem
        //**********************************************************************
        virtual void SetPosition(Vector3& center, float height, float topRadius, float bottomRadius) override
        {
            m_center = center;
            m_height = height;
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
                v.y = dy - height / 2.0f;
                v.z = 0.0f;
                pts.push_back(v);
                dy += height / float(subdivisions);
                radius += (topRadius - bottomRadius) / float(subdivisions);
            }

            std::vector<CRefObj<IMaterialAttrib>> materials;
            CRefObj<IMaterialAttrib> spMaterial = CCausticFactory::Instance()->CreateMaterialAttrib();
            materials.push_back(spMaterial);
            FRGBColor diffuseClr(0.7f, 0.7f, 0.7f);
            spMaterial->SetColor(L"diffuseColor", diffuseClr);
            FRGBColor ambientClr(0.1f, 0.1f, 0.1f);
            spMaterial->SetColor(L"ambientColor", ambientClr);

            m_spMesh = Caustic::CreateSurfaceRevolution(pts, subdivisions, subdivisions, 360.0f);
            m_spMesh->SetMaterials(materials);
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
