//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <string>

export module Rendering.SceneGraph.SceneLightCollectionElem;
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
import Rendering.Caustic.ILight;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.RendererFlags;
import Rendering.SceneGraph.SceneGroupElem;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneMaterialElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneLightCollectionElem
    // Defines a group of lights that effect all its children
    //**********************************************************************
    class CSceneLightCollectionElem :
        public CSceneGroupElem,
        public ISceneLightCollectionElem
    {
        std::vector<CRefObj<ILight>> m_lights;
        static CRefObj<ISceneMaterialElem> sm_spLightProxyMaterialElem;
        static bool sm_proxiesCreated;

        CRefObj<ISceneMaterialElem> LoadLightProxies(IRenderer* pRenderer);
    public:
        CSceneLightCollectionElem()
        {
            m_passes = c_PassAllPasses;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override;

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::LightCollection; }
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
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override;
        virtual void GetBBox(BBox3* pBBox) override;
        virtual uint32_t GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32_t flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override
        {
        }

        virtual void Store(IStream* pStream) override
        {
        }

        //**********************************************************************
        // ISceneGroupElem
        //**********************************************************************
        virtual uint32_t NumberChildren() override { return CSceneGroupElem::NumberChildren(); }
        virtual CRefObj<ISceneElem> GetChild(uint32_t index) override { return CSceneGroupElem::GetChild(index); }
        virtual void AddChild(ISceneElem* pElem) override { return CSceneGroupElem::AddChild(pElem); }
        virtual void InsertChild(ISceneElem* pElem, uint32_t index) override { return CSceneGroupElem::InsertChild(pElem, index); }
        virtual Matrix4x4 GetTransform() override { return m_Transform; }
        virtual void SetTransform(Matrix4x4& transform) override { CSceneGroupElem::SetTransform(transform); }

        //**********************************************************************
        // ISceneLightCollectionElem
        //**********************************************************************
        virtual uint32_t NumberLights() override;
        virtual CRefObj<ILight> GetLight(int index) override;
        virtual void AddLight(ILight* pLight) override;
        virtual void RemoveLight(ILight* pLight) override;
    };
}
