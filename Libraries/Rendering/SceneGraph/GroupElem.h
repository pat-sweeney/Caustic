//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "ISceneGraph.h"
#include "SceneElem.h"
#include <functional>

namespace Caustic
{
    //**********************************************************************
    // Class: CSceneGroupElem
	// Defines a simple group of scene graph elements
    //**********************************************************************
    class CSceneGroupElem : 
        public CSceneElem, 
        public ISceneGroupElem, 
        public CRefCount
    {
    protected:
        std::vector<CRefObj<ISceneElem>> m_Children;
        Matrix4x4 m_Transform;
    public:
		//**********************************************************************
		// Method: Load
		// Loads a scene element from a stream
		//**********************************************************************
		virtual void Load(IStream *pStream, std::function<void(ISceneElem *pElem)> func);

		//**********************************************************************
		// Method: Store
		// Saves a scene element to a stream
		//**********************************************************************
		virtual void Store(IStream *pStream, std::function<void(ISceneElem *pElem)> func);

		//=================================
		// IUnknown
		//=================================

		//**********************************************************************
		// Method: AddRef
		// Increments the reference count on the scene element
		//**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// Method: Release
		// Decrements the reference count on the scene element
		//**********************************************************************
		virtual uint32 Release() override { return CRefCount::Release(); }

		//=================================
		// ISceneElem
		//=================================

		//**********************************************************************
		// Method: GetType
		// Returns the scene element's type
		//**********************************************************************
		virtual ESceneElemType GetType() override { return ESceneElemType::Group; }

		//**********************************************************************
		// Method: Name
		// Returns the scene element's name
		//**********************************************************************
		virtual std::wstring &Name() override;

		//**********************************************************************
		// Method: Render
		// Renders the scene element
		//
		// Parameters:
		// pRenderer - renderer to render to
		// pRenderCtx - current render context
		// pSceneCtx - current scene context
		//**********************************************************************
		virtual void Render(IRenderer *pRenderer, IRenderCtx *pRenderCtx, SceneCtx *pSceneCtx) override;

		//**********************************************************************
		// Method: GetBBox
		// Returns the bounding box of the scene element
		//
		// Parameters:
		// pBBox - Element's bounding box
		//**********************************************************************
		virtual void GetBBox(BBox3 *pBBox) override;

		//**********************************************************************
		// Method: GetFlags
		// Returns the flags associated with this scene element. See ESceneElemFlags.
		//
		// Returns:
		// Set of flags as defined by ESceneElemFlags
		//**********************************************************************
		virtual uint32 GetFlags() override { return m_Flags; }

		//**********************************************************************
		// Method: SetFlags
		// Sets the flags associated with this scene element. See ESceneElemFlags.
		//
		// Parameters:
		// flags - set of flags to set on the scene element
		//**********************************************************************
		virtual void SetFlags(uint32 flags) override { m_Flags = flags; }

		//==============================
		// ISerialize
		//==============================

		//**********************************************************************
		// Method: Load
		// Loads the group element from the specified stream
		//
		// Parameters:
		// pStream - stream to read from
		//**********************************************************************
		virtual void Load(IStream *pStream) override;

		//**********************************************************************
		// Method: Store
		// Saves the group element to the specified stream
		//
		// Parameters:
		// pStream - stream to write to
		//**********************************************************************
		virtual void Store(IStream *pStream) override;

		//=================================
		// ISceneGroupElem
		//=================================

		//**********************************************************************
		// Method: NumberChildren
		// Returns the number of scene elements in the group
		//
		// Returns:
		// Returns the number of children in the group
		//**********************************************************************
		virtual uint32 NumberChildren() override;

		//**********************************************************************
		// Method: GetChild
		// Returns the Nth child in the group
		//
		// Parameters:
		// index - index of element to return
		//
		// Returns:
		// Returns the element at index
		//**********************************************************************
		virtual CRefObj<ISceneElem> GetChild(uint32 index) override;

		//**********************************************************************
		// Method: AddChild
		// Adds a child element to the group
		//
		// Parameters:
		// pElem - element to add
		//**********************************************************************
		virtual void AddChild(ISceneElem *pElem) override;

		//**********************************************************************
		// Method: InsertChild
		// Adds a child element to the group at position 'index'
		//
		// Parameters:
		// pElem - element to add
		// index - index to insert element at
		//**********************************************************************
		virtual void InsertChild(ISceneElem *pElem, uint32 index) override;

		//**********************************************************************
		// Method: SetTransform
		// Sets the transform for the group
		//
		// Parameters:
		// transform - transform to apply to group (and its children)
		//**********************************************************************
		virtual void SetTransform(Matrix4x4 &transform) override { m_Transform = transform; }
    };
}
