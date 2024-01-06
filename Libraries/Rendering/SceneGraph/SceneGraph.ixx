//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <functional>
#include <Windows.h>
#include <vector>

export module Rendering.SceneGraph.SceneGraph;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.BBox;
import Base.Math.Ray;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneCameraCollectionElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneGraph
    // Defines our scene graph
    //**********************************************************************
    class CSceneGraph : public CSceneElem, public ISceneGraph, public CRefCount
    {
        bool m_locked;
        CRITICAL_SECTION m_cs;
        CRefObj<ISceneGroupElem> m_spRoot;
        CRefObj<IRenderer> m_spRenderer;
        std::vector<CRefObj<ISceneElem>> m_Selected; // List of elements in our scene graph that are currently selected
        uint32_t m_Flags;
        bool m_showProxyObjects;
        CRefObj<ISceneCameraCollectionElem> m_CameraCollection;

        void PathTraceInternal(PathTraceCtx* pCtx, int depth, Ray3& ray, FRGBColor* pRadiance);
    public:
        CSceneGraph(ISceneGroupElem *pGroup) :
            m_locked(false),
            m_showProxyObjects(false)
        {
            InitializeCriticalSection(&m_cs);
            m_spRoot = pGroup;
            m_CameraCollection = CreateCameraCollectionElem();
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
            CRefObj<IJSonObj> sp = m_spRoot->AsJson(pPropertyName, pParser);
            return sp;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::SceneGraph; }
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
        virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
        virtual void GetBBox(BBox3* pBBox) override;
        virtual uint32_t GetFlags() { return m_Flags; }
        virtual void SetFlags(uint32_t flags) { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Store(IStream *pStream) override;
        virtual void Load(IStream *pStream) override;

        //**********************************************************************
        // ISceneGroupElem
        //**********************************************************************
        virtual uint32_t NumberChildren() override;
        virtual CRefObj<ISceneElem> GetChild(uint32_t index) override;
        virtual void AddChild(ISceneElem *pElem) override;
        virtual void InsertChild(ISceneElem *pElem, uint32_t index) override;
        virtual Matrix4x4 GetTransform() override;
        virtual void SetTransform(Matrix4x4 &mat) override;

        //**********************************************************************
        // ISceneGraph
        //**********************************************************************
        virtual bool GetShowProxyObjects() override { return m_showProxyObjects; }
        virtual void SetShowProxyObjects(bool show) override { m_showProxyObjects = show; }
        virtual void PathTrace(IRenderer *pRenderer, PathTraceCtx* pCtx, IImage* pDest) override;
        virtual void Merge(ISceneGraph *pGraph) override;
        virtual void Lock() override;
        virtual void Unlock() override;
        virtual void ClearSelected() override;
        virtual void SelectObject(ISceneElem* pElem) override;
        virtual void DeselectObject(ISceneElem* pElem) override;
        virtual CRefObj<ISceneCameraCollectionElem> GetCameras() override
        {
            return m_CameraCollection;
        }
    };
}
