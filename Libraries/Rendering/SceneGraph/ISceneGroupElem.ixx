//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneGroupElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneGroupElem
	// Defines a group element in our scene graph. Groups contain other scene elements.
	//**********************************************************************
	struct ISceneGroupElem : public ISceneElem
	{
		//**********************************************************************
		// Method: NumberChildren
		// Returns the number of child elements part of this group
		//**********************************************************************
		virtual uint32 NumberChildren() = 0;

		//**********************************************************************
		// Method: GetChild
		// Gets the Nth child element
		//**********************************************************************
		virtual CRefObj<ISceneElem> GetChild(uint32 index) = 0;

		//**********************************************************************
		// Method: AddChild
		// Adds a new scene element to the group
		//**********************************************************************
		virtual void AddChild(ISceneElem* pElem) = 0;

		//**********************************************************************
		// Method: InsertChild
		// Adds a new scene element to the group. The order of elements defines the render order.
		//**********************************************************************
		virtual void InsertChild(ISceneElem* pElem, uint32 index) = 0;

		//**********************************************************************
		// Method: GetTransform
		// Gets the transform associated with this group.
		//**********************************************************************
		virtual Matrix4x4 GetTransform() = 0;

		//**********************************************************************
		// Method: SetTransform
		// Sets the transform associated with this group.
		//**********************************************************************
		virtual void SetTransform(Matrix4x4& transform) = 0;
	};
	
	CRefObj<ISceneGroupElem> CreateGroupElem();
}
