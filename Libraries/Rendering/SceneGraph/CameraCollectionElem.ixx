//**********************************************************************
// Copyright Patrick Sweeney 2022-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <string>

export module Rendering.SceneGraph.SceneCameraCollectionElem;
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
import Rendering.Caustic.ICamera;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.RendererFlags;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneCameraCollectionElem;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneMaterialElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneCameraCollectionElem
    // Defines a group of Cameras that effect all its children
    //**********************************************************************
    class CSceneCameraCollectionElem :
        public CSceneElem,
        public CRefCount,
        public ISceneCameraCollectionElem
    {
        std::vector<CRefObj<ICamera>> m_Cameras;
        static CRefObj<ISceneMaterialElem> sm_spCameraProxyMaterialElem;
        static bool sm_proxiesCreated;

        CRefObj<ISceneMaterialElem> LoadCameraProxies(IRenderer* pRenderer);
    public:
        CSceneCameraCollectionElem()
        {
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            return CRefObj<IJSonObj>(nullptr);
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::CameraCollection; }
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
        virtual void GetBBox(BBox3* pBBox) { CSceneElem::GetBBox(pBBox); };
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
        // ISceneCameraCollectionElem
        //**********************************************************************
        virtual uint32_t NumberCameras() override;
        virtual CRefObj<ICamera> GetCamera(int index) override;
        virtual void AddCamera(ICamera* pCamera) override;
        virtual void RemoveCamera(ICamera* pCamera) override;
    };
}
