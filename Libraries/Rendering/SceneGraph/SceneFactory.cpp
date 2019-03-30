//**********************************************************************
// Copyright Patrick Sweeney 2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"
#include "ISceneFactory.h"
#include "SceneFactory.h"
#include "GroupElem.h"

namespace Caustic
{
	CAUSTICAPI void CreateSceneFactory(ISceneFactory **ppElem)
	{
		CSceneFactory *pFactory = new CSceneFactory();
		*ppElem = pFactory;
		(*ppElem)->AddRef();
	}
	
	CRefObj<ISceneFactory> CSceneFactory::s_factory;

	CAUSTICAPI void CreateSceneGraph(ISceneGraph **ppGraph);
	void CSceneFactory::CreateSceneGraph(ISceneGraph **ppGraph)
	{
		Caustic::CreateSceneGraph(ppGraph);
	}

	CAUSTICAPI void CreateMaterialElem(ISceneMaterialElem **ppElem);
	void CSceneFactory::CreateMaterialElem(ISceneMaterialElem **ppElem)
	{
		Caustic::CreateMaterialElem(ppElem);
	}

	CAUSTICAPI void CreatePointLightElem(IScenePointLightElem **ppLight);
	void CSceneFactory::CreatePointLightElem(IScenePointLightElem **ppLight)
	{
		Caustic::CreatePointLightElem(ppLight);
	}

	void CSceneFactory::CreateGroupElem(ISceneGroupElem **ppGroup)
	{
		std::unique_ptr<CSceneGroupElem> spGroupObj(new CSceneGroupElem());
		*ppElem = spGroupObj.release();
		(*ppElem)->AddRef();
	}

	CAUSTICAPI void CreateMeshElem(ISceneMeshElem **ppMesh);
	void CSceneFactory::CreateMeshElem(ISceneMeshElem **ppMesh)
	{
		Caustic::CreateMeshElem(ppMesh);
	}
};
