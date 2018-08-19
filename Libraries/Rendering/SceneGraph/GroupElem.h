//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "SceneGraphImpl.h"
#include <functional>

namespace Caustic
{
    ///**********************************************************************
    /// \brief CSceneGroupElem defines a simple group of scene graph elements
    ///**********************************************************************
    class CSceneGroupElem : 
        public CSceneElem, 
        public ISceneGroupElem, 
        public CRefCount
    {
    protected:
        std::vector<CRefObj<ISceneElem>> m_Children;
        Matrix4x4 m_Transform;
    public:
        virtual void Load(IStream *pStream, std::function<void(ISceneElem *pElem)> func);
        virtual void Store(IStream *pStream, std::function<void(ISceneElem *pElem)> func);

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual ESceneElemType GetType() override { return ESceneElemType::Group; }
        virtual std::wstring &Name() override;
        virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;
        virtual void GetBBox(BBox3 *pBBox) override;
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream *pStream) override;
        virtual void Store(IStream *pStream) override;

        //**********************************************************************
        // ISceneGroupElem
        //**********************************************************************
        virtual uint32 NumberChildren() override;
        virtual CRefObj<ISceneElem> GetChild(uint32 index) override;
        virtual void AddChild(ISceneElem *pElem) override;
        virtual void InsertChild(ISceneElem *pElem, uint32 index) override;
        virtual void SetTransform(Matrix4x4 &transform) override { m_Transform = transform; }
    };
}
