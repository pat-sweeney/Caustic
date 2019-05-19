//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Base\Math\Ray.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering/SceneGraph/ISceneGraph.h"
#include <vector>
#include <string>

#include "SceneElem.h"
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
        virtual std::wstring &Name() override { static std::wstring wstr(L"SceneGraph"); return wstr; }
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
