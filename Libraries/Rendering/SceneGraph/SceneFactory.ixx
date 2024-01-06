//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Rendering.SceneGraph.SceneFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Shader;
import Base.Math.Matrix;
import Base.Math.Vector;
import Rendering.SceneGraph.ISceneFactory;

export namespace Caustic
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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneFactory
        //**********************************************************************
        virtual CRefObj<ISceneGraph> CreateSceneGraph() override;
        virtual CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader) override;
        virtual CRefObj<ISceneMaterialElem> CreateMaterialElem() override;
        virtual CRefObj<ISceneLevelOfDetailElem> CreateLevelOfDetailElem() override;
        virtual CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem() override;
        virtual CRefObj<ISceneCameraCollectionElem> CreateCameraCollectionElem() override;
        virtual CRefObj<ISceneGroupElem> CreateGroupElem() override;
        virtual CRefObj<ISceneLineElem> CreateLineElem(Vector3 &p0, Vector3 &p1) override;
        virtual CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32_t subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed) override;
        virtual CRefObj<ISceneSphereElem> CreateSphereElem(Vector3 &center, float radius) override;
        virtual CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth) override;
        virtual CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius) override;
        virtual CRefObj<ISceneElem> CreateInstanceElem(ISceneElem *pTemplate) override;

        virtual CRefObj<ISceneMeshElem> CreateMeshElem() override;
        virtual CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer *pRender, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback) override;
        virtual CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader *pShader = nullptr) override;
    };
};
