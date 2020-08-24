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
	CAUSTICAPI CRefObj<ISceneFactory> CreateSceneFactory()
	{
		return CRefObj<ISceneFactory>(new CSceneFactory());
	}
	
	CRefObj<ISceneFactory> CSceneFactory::s_factory;

	CAUSTICAPI CRefObj<ISceneGraph> CreateSceneGraph();
	CRefObj<ISceneGraph> CSceneFactory::CreateSceneGraph()
	{
		return Caustic::CreateSceneGraph();
	}

	CAUSTICAPI CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader);
	CRefObj<ISceneComputeShaderElem> CSceneFactory::CreateComputeShaderElem(IShader *pComputeShader)
	{
		return Caustic::CreateComputeShaderElem(pComputeShader);
	}

	CAUSTICAPI CRefObj<ISceneMaterialElem> CreateMaterialElem();
	CRefObj<ISceneMaterialElem> CSceneFactory::CreateMaterialElem()
	{
		return Caustic::CreateMaterialElem();
	}

	CAUSTICAPI CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem();
	CRefObj<ISceneLightCollectionElem> CSceneFactory::CreateLightCollectionElem()
	{
		return Caustic::CreateLightCollectionElem();
	}

	CAUSTICAPI CRefObj<ISceneGroupElem> CreateGroupElem();
	CRefObj<ISceneGroupElem> CSceneFactory::CreateGroupElem()
	{
		return Caustic::CreateGroupElem();
	}

	CAUSTICAPI CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader *pShader = nullptr);
	CRefObj<ISceneOverlay2DElem> CSceneFactory::CreateOverlay2DElem(IShader *pShader /* = nullptr */)
	{
		return Caustic::CreateOverlay2DElem(pShader);
	}

	CAUSTICAPI CRefObj<ISceneMeshElem> CreateMeshElem();
	CRefObj<ISceneMeshElem> CSceneFactory::CreateMeshElem()
	{
		return Caustic::CreateMeshElem();
	}

	CAUSTICAPI CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback);
	CRefObj<ISceneCustomRenderElem> CSceneFactory::CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pCtx, SceneCtx * pSceneCtx)> clientCallback)
	{
		return Caustic::CreateCustomRenderElem(clientCallback);
	}
};
