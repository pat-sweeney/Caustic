//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "ISceneFactory.h"
#include "SceneFactory.h"
import Base.Core.Core;
import Rendering.Caustic.Shader;
import Base.Math.Matrix;

namespace Caustic
{
	CRefObj<ISceneFactory> CreateSceneFactory()
	{
		return CRefObj<ISceneFactory>(new CSceneFactory());
	}
	
	CRefObj<ISceneFactory> CSceneFactory::s_factory;

	CRefObj<ISceneGraph> CreateSceneGraph();
	CRefObj<ISceneGraph> CSceneFactory::CreateSceneGraph()
	{
		return Caustic::CreateSceneGraph();
	}

	CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader);
	CRefObj<ISceneComputeShaderElem> CSceneFactory::CreateComputeShaderElem(IShader *pComputeShader)
	{
		return Caustic::CreateComputeShaderElem(pComputeShader);
	}

	CRefObj<ISceneMaterialElem> CreateMaterialElem();
	CRefObj<ISceneMaterialElem> CSceneFactory::CreateMaterialElem()
	{
		return Caustic::CreateMaterialElem();
	}

	CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem();
	CRefObj<ISceneLightCollectionElem> CSceneFactory::CreateLightCollectionElem()
	{
		return Caustic::CreateLightCollectionElem();
	}

	CRefObj<ISceneGroupElem> CreateGroupElem();
	CRefObj<ISceneGroupElem> CSceneFactory::CreateGroupElem()
	{
		return Caustic::CreateGroupElem();
	}

	CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader *pShader = nullptr);
	CRefObj<ISceneOverlay2DElem> CSceneFactory::CreateOverlay2DElem(IShader *pShader /* = nullptr */)
	{
		return Caustic::CreateOverlay2DElem(pShader);
	}

	CRefObj<ISceneMeshElem> CreateMeshElem();
	CRefObj<ISceneMeshElem> CSceneFactory::CreateMeshElem()
	{
		return Caustic::CreateMeshElem();
	}

	CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed);
	CRefObj<ISceneMarchingCubesElem> CSceneFactory::CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed)
	{
		return Caustic::CreateMarchingCubesElem(pRenderer, subdivisions, sdf, drawIndexed);
	}

	CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback);
	CRefObj<ISceneCustomRenderElem> CSceneFactory::CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pCtx, SceneCtx * pSceneCtx)> clientCallback)
	{
		return Caustic::CreateCustomRenderElem(clientCallback);
	}

	CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1);
	CRefObj<ISceneLineElem> CSceneFactory::CreateLineElem(Vector3& p0, Vector3& p1)
	{
		return Caustic::CreateLineElem(p0, p1);
	}

	CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius);
	CRefObj<ISceneSphereElem> CSceneFactory::CreateSphereElem(Vector3& center, float radius)
	{
		return Caustic::CreateSphereElem(center, radius);
	}

	CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth);
	CRefObj<ISceneCubeElem> CSceneFactory::CreateCubeElem(Vector3& center, float width, float height, float depth)
	{
		return Caustic::CreateCubeElem(center, width, height, depth);
	}

	CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius);
	CRefObj<ISceneCylinderElem> CSceneFactory::CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
	{
		return Caustic::CreateCylinderElem(center, height, topRadius, bottomRadius);
	}

//	CRefObj<ISceneElem> CreateInstanceElem(ISceneElem* pTemplate);
	CRefObj<ISceneElem> CSceneFactory::CreateInstanceElem(ISceneElem* pTemplate)
	{
		return CRefObj<ISceneElem>(nullptr);
//		return Caustic::CreateInstanceElem(pTemplate);
	}
};
