//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"
#include "ISceneGraph.h"
#include "ISceneFactory.h"

namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneFactory
	// Factory used for creating objects that make up parts of our scene graph
	//**********************************************************************
	struct ISceneFactory : public IRefCount
	{
		//**********************************************************************
		// Method: CreateMaterialElem
		// Creates a material element
		//**********************************************************************
		virtual CRefObj<ISceneMaterialElem> CreateMaterialElem() = 0;

		//**********************************************************************
		// Method: CreateMaterialElem
		// Creates a material element
		//**********************************************************************
		virtual CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader) = 0;

		//**********************************************************************
		// Method: CreateLightCollectionElem
		// Creates a light collection element
		//**********************************************************************
		virtual CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem() = 0;

		//**********************************************************************
		// Method: CreateGroupElem
		// Creates a group element
		//**********************************************************************
		virtual CRefObj<ISceneGroupElem> CreateGroupElem() = 0;

		//**********************************************************************
		// Method: CreateMeshElem
		// Creates a mesh element
		//**********************************************************************
		virtual CRefObj<ISceneMeshElem> CreateMeshElem() = 0;

		//**********************************************************************
		// Method: CreateCustomRenderElem
		// Create a element that will use a callback for rendering
		//**********************************************************************
		virtual CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer* pRender, IRenderCtx* pCtx, SceneCtx* pSceneCtx)> clientCallback) = 0;

		//**********************************************************************
		// Method: CreateSceneGraph
		// Creates a new scene graph
		//**********************************************************************
		virtual CRefObj<ISceneGraph> CreateSceneGraph() = 0;
	};

	//**********************************************************************
	// Function: CreateSceneFactory
	// Creates a scene factory. This factory is used to create elements in
	// the scene graph.
	//**********************************************************************
	CAUSTICAPI CRefObj<ISceneFactory> CreateSceneFactory();
};
