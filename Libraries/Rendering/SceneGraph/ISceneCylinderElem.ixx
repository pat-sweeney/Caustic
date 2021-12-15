//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneCylinderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneCylinderElem
	// Defines a cylinder element in our scene graph.
	//**********************************************************************
	struct ISceneCylinderElem : public ISceneElem
	{
		virtual void SetPosition(Vector3& center, float height, float topRadius, float bottomRadius) = 0;
		virtual void GetPosition(Vector3* pCenter, float* pHeight, float* pTopRadius, float* pBottomRadius) = 0;
	};

	CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius);
}
