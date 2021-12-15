//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.ISceneComputeShaderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IShader;
import Rendering.SceneGraph.SceneFactory;
import Rendering.SceneGraph.SceneComputeShaderElem;

namespace Caustic
{
	CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader)
	{
		return CRefObj<ISceneComputeShaderElem>(new CSceneComputeShaderElem(pComputeShader));
	}
}
