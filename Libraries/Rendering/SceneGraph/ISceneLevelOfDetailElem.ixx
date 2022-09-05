//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneLevelOfDetailElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneLevelOfDetailElem
	// Defines an element in our scene graph for handling level of detail
	//**********************************************************************
	struct ISceneLevelOfDetailElem : public ISceneElem
	{
		//**********************************************************************
		// Method: SetMinRenderDistance
		// Sets the minimum distance from the object to the camera before rendering
		// of the object occurs
		// 
		// Parameters:
		// childIndex - index of object
		// dist - distance in meters
		//**********************************************************************
		virtual void SetMinRenderDistance(int childIndex, float dist) = 0;

		//**********************************************************************
		// Method: SetMaxRenderDistance
		// Sets the maximum distance from the object to the camera after rendering
		// of the object stops
		// 
		// Parameters:
		// childIndex - index of object
		// dist - distance in meters
		//**********************************************************************
		virtual void SetMaxRenderDistance(int childIndex, float dist) = 0;

		//**********************************************************************
		// Method: GetMinRenderDistance
		// Gets the minimum distance from the object to the camera before rendering
		// of the object occurs
		// 
		// Parameters:
		// childIndex - index of object
		// 
		// Returns:
		// Minimum distance from camera before object renders
		//**********************************************************************
		virtual float GetMinRenderDistance(int childIndex) = 0;

		//**********************************************************************
		// Method: GetMaxRenderDistance
		// Gets the maximum distance from the object to the camera after rendering
		// of the object stops
		// 
		// Parameters:
		// childIndex - index of object
		// 
		// Returns:
		// Maximum distance from camera after which object stops rendering
		//**********************************************************************
		virtual float GetMaxRenderDistance(int childIndex) = 0;

		//**********************************************************************
		// Method: NumberChildren
		// Returns the number of child elements part of this group
		//**********************************************************************
		virtual uint32 NumberChildren() = 0;

		//**********************************************************************
		// Method: GetChild
		// Gets the Nth child element
		// 
		// Parameters:
		// index - index of child element
		//**********************************************************************
		virtual CRefObj<ISceneElem> GetChild(uint32 index) = 0;

		//**********************************************************************
		// Method: AddChild
		// Adds a new child to the level of detail group
		// 
		// Parameters:
		// pElem - element to add
		// minRenderDistance - minimum distance of object from camera for rendering to occur
		// maxRenderDistance - maximum distance of object from camera for rendering to occur
		//**********************************************************************
		virtual void AddChild(ISceneElem* pElem, float minRenderDistance, float maxRenderDistance) = 0;
	};

	CRefObj<ISceneLevelOfDetailElem> CreateLevelOfDetailElem();
}
