//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneCubeElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.SceneGraph.SceneCubeElem;

namespace Caustic
{
	CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth)
	{
		return CRefObj<ISceneCubeElem>(new CSceneCubeElem(center, width, height, depth));
	}
}
