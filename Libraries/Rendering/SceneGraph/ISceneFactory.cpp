//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneFactory;

namespace Caustic
{
	CRefObj<ISceneFactory> CreateSceneFactory()
	{
		return CRefObj<ISceneFactory>(new CSceneFactory());
	}
}
