//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
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
    //**********************************************************************
    // Class: CSceneCustomRenderElem
    // Defines a mesh element in our scene graph
    //**********************************************************************
    class CSceneCustomRenderElem :
        public CSceneElem,
        public ISceneCustomRenderElem,
        public CRefCount
    {
        std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx *pSceneCtx)> m_clientCallback;
    public:
        //**********************************************************************
        // Constructor: CSceneCustomRenderElem
        // Default constructor
        //**********************************************************************
        CSceneCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx * pSceneCtx)> clientCallback)
        {
            m_clientCallback = clientCallback;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual ESceneElemType GetType() { return ESceneElemType::Mesh; }
        virtual std::wstring& Name() override { return CSceneElem::m_Name;};
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override
        {
            m_clientCallback(pRenderer, pRenderCtx, pSceneCtx);
        }
        virtual void GetBBox(BBox3* pBBox) override { return; }
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { return; }
        virtual void Store(IStream* pStream) override { return; }

        //**********************************************************************
        // ISceneCustomRenderElem
        //**********************************************************************
        virtual void SetCallback(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback) override
        {
            m_clientCallback = clientCallback;
        }
    };
};