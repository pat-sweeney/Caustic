//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneSphereElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Base.Math.Vector;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneSphereElem
	// Defines a sphere element in our scene graph.
	//**********************************************************************
	struct ISceneSphereElem : public ISceneElem
	{
		//**********************************************************************
		// Methods: SetPosition
		// Sets the center+radius of a sphere
		// 
		// Parameters:
		// center - center of sphere
		// radius - radius of sphere in meters
		//**********************************************************************
		virtual void SetPosition(Vector3& center, float& radius) = 0;

		//**********************************************************************
		// Methods: GetPosition
		// Returns the center+radius of a sphere
		// 
		// Parameters:
		// pCenter - center of sphere
		// pRadius - radius of sphere in meters
		//**********************************************************************
		virtual void GetPosition(Vector3* pCenter, float* pRadius) = 0;
	};

	CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius);
}
