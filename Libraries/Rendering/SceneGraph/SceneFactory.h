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
		virtual CRefObj<ISceneMeshElem> CreateMeshElem() override;
		virtual CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRender, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback) override;
	};
};
