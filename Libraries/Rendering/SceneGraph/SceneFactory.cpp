//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.SceneGraph.SceneFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Shader;
import Base.Math.Matrix;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneComputeShaderElem;
import Rendering.SceneGraph.ISceneMaterialElem;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneOverlay2DElem;
import Rendering.SceneGraph.ISceneMeshElem;
import Rendering.SceneGraph.ISceneMarchingCubesElem;
import Rendering.SceneGraph.ISceneCustomRenderElem;
import Rendering.SceneGraph.ISceneLineElem;
import Rendering.SceneGraph.ISceneSphereElem;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneCylinderElem;
import Rendering.SceneGraph.ISceneLevelOfDetailElem;

namespace Caustic
{
    CRefObj<ISceneFactory> CSceneFactory::s_factory;

    CRefObj<ISceneGraph> CSceneFactory::CreateSceneGraph()
    {
        return Caustic::CreateSceneGraph();
    }

    CRefObj<ISceneComputeShaderElem> CSceneFactory::CreateComputeShaderElem(IShader *pComputeShader)
    {
        return Caustic::CreateComputeShaderElem(pComputeShader);
    }

    CRefObj<ISceneLevelOfDetailElem> CSceneFactory::CreateLevelOfDetailElem()
    {
        return Caustic::CreateLevelOfDetailElem();
    }

    CRefObj<ISceneMaterialElem> CSceneFactory::CreateMaterialElem()
    {
        return Caustic::CreateMaterialElem();
    }

    CRefObj<ISceneLightCollectionElem> CSceneFactory::CreateLightCollectionElem()
    {
        return Caustic::CreateLightCollectionElem();
    }

    CRefObj<ISceneCameraCollectionElem> CSceneFactory::CreateCameraCollectionElem()
    {
        return Caustic::CreateCameraCollectionElem();
    }

    CRefObj<ISceneGroupElem> CSceneFactory::CreateGroupElem()
    {
        return Caustic::CreateGroupElem();
    }

    CRefObj<ISceneOverlay2DElem> CSceneFactory::CreateOverlay2DElem(IShader *pShader /* = nullptr */)
    {
        return Caustic::CreateOverlay2DElem(pShader);
    }

    CRefObj<ISceneMeshElem> CSceneFactory::CreateMeshElem()
    {
        return Caustic::CreateMeshElem();
    }

    CRefObj<ISceneMarchingCubesElem> CSceneFactory::CreateMarchingCubesElem(IRenderer* pRenderer, uint32_t subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed)
    {
        return Caustic::CreateMarchingCubesElem(pRenderer, subdivisions, sdf, drawIndexed);
    }

    CRefObj<ISceneCustomRenderElem> CSceneFactory::CreateCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pCtx, SceneCtx * pSceneCtx)> clientCallback)
    {
        return Caustic::CreateCustomRenderElem(clientCallback);
    }

    CRefObj<ISceneLineElem> CSceneFactory::CreateLineElem(Vector3& p0, Vector3& p1)
    {
        return Caustic::CreateLineElem(p0, p1);
    }

    CRefObj<ISceneSphereElem> CSceneFactory::CreateSphereElem(Vector3& center, float radius)
    {
        return Caustic::CreateSphereElem(center, radius);
    }

    CRefObj<ISceneCubeElem> CSceneFactory::CreateCubeElem(Vector3& center, float width, float height, float depth)
    {
        return Caustic::CreateCubeElem(center, width, height, depth);
    }

    CRefObj<ISceneCylinderElem> CSceneFactory::CreateCylinderElem(Vector3& center, float height, float topRadius, float bottomRadius)
    {
        return Caustic::CreateCylinderElem(center, height, topRadius, bottomRadius);
    }

//    CRefObj<ISceneElem> CreateInstanceElem(ISceneElem* pTemplate);
    CRefObj<ISceneElem> CSceneFactory::CreateInstanceElem(ISceneElem* pTemplate)
    {
        return CRefObj<ISceneElem>(nullptr);
//        return Caustic::CreateInstanceElem(pTemplate);
    }
};
