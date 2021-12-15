//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Rendering.SceneGraph.ISceneMarchingCubesElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IShader;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneMarchingCubesElem
	// Implements marching cubes on a signed distance function
	//**********************************************************************
	struct ISceneMarchingCubesElem : public ISceneElem
	{
		virtual CRefObj<IShader> GetShader() = 0;
		virtual void SetSubdivisions(uint32 numCells) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
		virtual void SetSignedDistanceFunction(std::function<float(Vector3& v)> fn) = 0;
	};

	CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed);
}
