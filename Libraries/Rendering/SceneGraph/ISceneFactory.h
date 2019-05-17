//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"
#include "SceneGraph.h"

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
		virtual void CreateMaterialElem(ISceneMaterialElem **ppElem) = 0;

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
		virtual void CreateMeshElem(ISceneMeshElem **ppMesh) = 0;

		//**********************************************************************
		// Method: CreateSceneGraph
		// Creates a new scene graph
		//**********************************************************************
		virtual void CreateSceneGraph(ISceneGraph **ppGraph) = 0;
	};
}

//**********************************************************************
// Namespace: Caustic
//**********************************************************************
namespace Caustic
{
	//**********************************************************************
	// Function: CreateSceneFactory
	// Creates a scene factory. This factory is used to create elements in
	// the scene graph.
	//**********************************************************************
	CAUSTICAPI void CreateSceneFactory(ISceneFactory **ppElem);
}