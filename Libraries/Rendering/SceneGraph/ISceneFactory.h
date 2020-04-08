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
		virtual void CreateMaterialElem(Caustic::ISceneMaterialElem **ppElem) = 0;

		//**********************************************************************
		// Method: CreatePointLightElem
		// Creates a point light element
		//**********************************************************************
		virtual void CreatePointLightElem(IScenePointLightElem **ppLight) = 0;

		//**********************************************************************
		// Method: CreateGroupElem
		// Creates a group element
		//**********************************************************************
		virtual void CreateGroupElem(ISceneGroupElem **ppGroup) = 0;

		//**********************************************************************
		// Method: CreateMeshElem
		// Creates a mesh element
		//**********************************************************************
		virtual void CreateMeshElem(ISceneMeshElem** ppMesh) = 0;

		//**********************************************************************
		// Method: CreateCustomRenderElem
		// Create a element that will use a callback for rendering
		//**********************************************************************
		virtual void CreateCustomRenderElem(std::function<void(IRenderer *pRenderer, IRenderCtx	 *pCtx, SceneCtx * pSceneCtx)> clientCallback, ISceneCustomRenderElem** ppMesh) = 0;

		//**********************************************************************
		// Method: CreateSceneGraph
		// Creates a new scene graph
		//**********************************************************************
		virtual void CreateSceneGraph(ISceneGraph **ppGraph) = 0;
	};

	//**********************************************************************
	// Function: CreateSceneFactory
	// Creates a scene factory. This factory is used to create elements in
	// the scene graph.
	//**********************************************************************
	CAUSTICAPI void CreateSceneFactory(ISceneFactory **ppElem);
};
