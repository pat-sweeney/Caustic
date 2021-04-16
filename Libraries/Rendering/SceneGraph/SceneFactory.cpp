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

	CAUSTICAPI CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed);
	CRefObj<ISceneMarchingCubesElem> CSceneFactory::CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed)
	{
		return Caustic::CreateMarchingCubesElem(pRenderer, subdivisions, sdf, drawIndexed);
	}

	CAUSTICAPI CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback);
	CRefObj<ISceneCustomRenderElem> CSceneFactory::CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pCtx, SceneCtx * pSceneCtx)> clientCallback)
	{
		return Caustic::CreateCustomRenderElem(clientCallback);
	}

	CAUSTICAPI CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1);
	CRefObj<ISceneLineElem> CSceneFactory::CreateLineElem(Vector3& p0, Vector3& p1)
	{
		return Caustic::CreateLineElem(p0, p1);
	}

	CAUSTICAPI CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius);
	CRefObj<ISceneSphereElem> CSceneFactory::CreateSphereElem(Vector3& center, float radius)
	{
		return Caustic::CreateSphereElem(center, radius);
	}

	CAUSTICAPI CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth);
	CRefObj<ISceneCubeElem> CSceneFactory::CreateCubeElem(Vector3& center, float width, float height, float depth)
	{
		return Caustic::CreateCubeElem(center, width, height, depth);
	}

	CAUSTICAPI CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius);
	CRefObj<ISceneCylinderElem> CSceneFactory::CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
	{
		return Caustic::CreateCylinderElem(center, height, topRadius, bottomRadius);
	}

//	CAUSTICAPI CRefObj<ISceneElem> CreateInstanceElem(ISceneElem* pTemplate);
	CRefObj<ISceneElem> CSceneFactory::CreateInstanceElem(ISceneElem* pTemplate)
	{
		return CRefObj<ISceneElem>(nullptr);
//		return Caustic::CreateInstanceElem(pTemplate);
	}
};
