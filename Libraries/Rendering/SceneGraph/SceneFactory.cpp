//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
#include "Base\Math\Matrix.h"
#include "ISceneFactory.h"
#include "SceneFactory.h"

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

	CAUSTICAPI void CreateGroupElem(ISceneGroupElem **ppGroup);
	void CSceneFactory::CreateGroupElem(ISceneGroupElem **ppGroup)
	{
		Caustic::CreateGroupElem(ppGroup);
	}

	CAUSTICAPI void CreateMeshElem(ISceneMeshElem** ppMesh);
	void CSceneFactory::CreateMeshElem(ISceneMeshElem** ppMesh)
	{
		Caustic::CreateMeshElem(ppMesh);
	}

	CAUSTICAPI void CreateCustomRenderElem(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback, ISceneCustomRenderElem** ppRenderElem);
	void CSceneFactory::CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pCtx, SceneCtx * pSceneCtx)> clientCallback, ISceneCustomRenderElem** ppRenderElem)
	{
		Caustic::CreateCustomRenderElem(clientCallback, ppRenderElem);
	}
};
