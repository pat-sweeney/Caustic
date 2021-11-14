//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\Shader.h"
import Base.Math.Matrix;
#include "ISceneFactory.h"
#include "ISceneGraph.h"

namespace Caustic
{
	class CSceneFactory : public ISceneFactory, public CRefCount
	{
		static CRefObj<ISceneFactory> s_factory;

	public:
		static CRefObj<ISceneFactory> Instance()
		{
			if (s_factory == nullptr)
			{
				s_factory = CRefObj<ISceneFactory>(new CSceneFactory());
			}
			return s_factory;
		}

		CSceneFactory()
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// ISceneFactory
		//**********************************************************************
		virtual CRefObj<ISceneGraph> CreateSceneGraph() override;
		virtual CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader) override;
		virtual CRefObj<ISceneMaterialElem> CreateMaterialElem() override;
		virtual CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem() override;
		virtual CRefObj<ISceneGroupElem> CreateGroupElem() override;
		virtual CRefObj<ISceneLineElem> CreateLineElem(Vector3 &p0, Vector3 &p1) override;
		virtual CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed) override;
		virtual CRefObj<ISceneSphereElem> CreateSphereElem(Vector3 &center, float radius) override;
		virtual CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth) override;
		virtual CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius) override;
		virtual CRefObj<ISceneElem> CreateInstanceElem(ISceneElem *pTemplate) override;

		virtual CRefObj<ISceneMeshElem> CreateMeshElem() override;
		virtual CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRender, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback) override;
		virtual CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader *pShader = nullptr) override;
	};
};
