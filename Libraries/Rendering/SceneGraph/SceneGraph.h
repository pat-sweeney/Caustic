//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Ray.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\SceneGraph\ISceneGraph.h"
#include <vector>
#include <string>

namespace Caustic
{
    //**********************************************************************
    // Class: CSceneElem
	// Defines the base class for all our scene graph elements
    //**********************************************************************
    class CSceneElem
    {
    protected:
        std::wstring m_Name; // User provided name for this element
        uint32 m_Flags;
        BBox3 m_BBox;
        std::function<bool(int pass)> m_prerenderCallback;
        std::function<void(int pass)> m_postrenderCallback;

        void DrawSelected(IRenderer *pRenderer, ISceneElem *pElem, SceneCtx *pSceneCtx);
    public:
        CSceneElem() :
            m_Flags(0)
        {
        }

        //**********************************************************************
        // ISceneElem base implementation
        //**********************************************************************
        ESceneElemType GetType() { return ESceneElemType::Unknown; }
        std::wstring GetName() { return m_Name; }
        void SetName(const wchar_t *name) { m_Name = name; }
        void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback)
        {
            m_prerenderCallback = prerenderCallback;
        }
        void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback)
        {
            m_postrenderCallback = postrenderCallback;
        }
        void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
        {
            CT(E_UNEXPECTED);
        }
        void GetBBox(BBox3* pBBox) { *pBBox = m_BBox; }
        uint32 GetFlags() { return m_Flags; }
        void SetFlags(uint32 flags) { m_Flags = flags; }
    };
}
#include "GroupElem.h"

namespace Caustic
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
    public:
        CSceneGraph(ISceneGroupElem *pGroup) :
            m_locked(false)
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
        virtual void Merge(ISceneGraph *pGraph) override;
        virtual void Lock() override;
        virtual void Unlock() override;
//        virtual void SelectObject(Ray3 &ray, ISceneElem **ppElem);
    };
}
