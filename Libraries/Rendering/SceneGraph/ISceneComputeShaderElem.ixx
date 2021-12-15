//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneComputeShaderElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;
import Rendering.SceneGraph.ISceneElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneComputeShaderElem
	// Defines a compute shader in our scene graph. Compute shaders are run
	// during scene graph traversal but before any rendering elements.
	//**********************************************************************
	struct ISceneComputeShaderElem : public ISceneElem
	{
		virtual CRefObj<IShader> GetShader() = 0;
		virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth = 1) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) = 0;
		virtual void SetShaderParam(const wchar_t* pParamName, float value) = 0;
		virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 elemSize) = 0;
		virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) = 0;
	};

	CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader);
}
