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
		virtual void SetPosition(Vector3& center, float& radius) = 0;
		virtual void GetPosition(Vector3* pCenter, float* pRadius) = 0;
	};

	CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius);
}
