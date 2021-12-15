//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneGraph;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.ISceneGroupElem;

namespace Caustic
{
	CRefObj<ISceneGraph> CreateSceneGraph()
	{
		CRefObj<ISceneGroupElem> spGroup = CreateGroupElem();
		return CRefObj<ISceneGraph>(new CSceneGraph(spGroup));
	}
}