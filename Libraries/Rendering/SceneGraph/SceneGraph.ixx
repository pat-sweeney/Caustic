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
        std::vector<CRefObj<ISceneElem>> m_Selected;
        uint32 m_Flags;
        bool m_showProxyObjects;

        void PathTraceInternal(PathTraceCtx* pCtx, int depth, Ray3& ray, FRGBColor* pRadiance);
    public:
        CSceneGraph(ISceneGroupElem *pGroup) :
            m_locked(false),
            m_showProxyObjects(false)
        {
            InitializeCriticalSection(&m_cs);
			m_spRoot = pGroup;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

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
        virtual void GetBBox(BBox3 *pBBox) override
        {
            pBBox->minPt = Vector3(0.0f, 0.0f, 0.0f);
            pBBox->maxPt = Vector3(0.0f, 0.0f, 0.0f);
        }
        virtual uint32 GetFlags() { return m_Flags; }
        virtual void SetFlags(uint32 flags) { m_Flags = flags; }
        virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Store(IStream *pStream) override;
        virtual void Load(IStream *pStream) override;

        //**********************************************************************
        // ISceneGroupElem
        //**********************************************************************
        virtual uint32 NumberChildren() override;
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override;
        virtual void AddChild(ISceneElem *pElem) override;
        virtual void InsertChild(ISceneElem *pElem, uint32 index) override;
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
//        virtual void SelectObject(Ray3 &ray, ISceneElem **ppElem);
    };
}
