//**********************************************************************
// Copyright Patrick Sweeney 2019-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.SceneGraph.ISceneFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.ISceneMaterialElem;
import Rendering.SceneGraph.ISceneComputeShaderElem;
import Rendering.SceneGraph.ISceneLightCollectionElem;
import Rendering.SceneGraph.ISceneCameraCollectionElem;
import Rendering.SceneGraph.ISceneGroupElem;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneMarchingCubesElem;
import Rendering.SceneGraph.ISceneMeshElem;
import Rendering.SceneGraph.ISceneLineElem;
import Rendering.SceneGraph.ISceneSphereElem;
import Rendering.SceneGraph.ISceneCubeElem;
import Rendering.SceneGraph.ISceneCylinderElem;
import Rendering.SceneGraph.ISceneCustomRenderElem;
import Rendering.SceneGraph.ISceneOverlay2DElem;
import Rendering.SceneGraph.ISceneGraph;

export namespace Caustic
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
		virtual CRefObj<ISceneMaterialElem> CreateMaterialElem() = 0;

		//**********************************************************************
		// Method: CreateMaterialElem
		// Creates a material element
		//**********************************************************************
		virtual CRefObj<ISceneComputeShaderElem> CreateComputeShaderElem(IShader* pComputeShader) = 0;

		//**********************************************************************
		// Method: CreateLightCollectionElem
		// Creates a light collection element
		//**********************************************************************
		virtual CRefObj<ISceneLightCollectionElem> CreateLightCollectionElem() = 0;

		//**********************************************************************
		// Method: CreateCameraCollectionElem
		// Creates a camera collection element
		//**********************************************************************
		virtual CRefObj<ISceneCameraCollectionElem> CreateCameraCollectionElem() = 0;

		//**********************************************************************
		// Method: CreateGroupElem
		// Creates a group element
		//**********************************************************************
		virtual CRefObj<ISceneGroupElem> CreateGroupElem() = 0;

		//**********************************************************************
		// Method: CreateMarchingCubesElem
		// Creates an element that uses the marching cubes algorithm to convert
		// a signed distance field into a mesh.
		// 
		// Parameters:
		// pRenderer - renderer to use
		// subdivisions - number of cells along each voxel grid axis
		// sdf - function that returns the signed distance field for the voxel grid
		// drawIndexed - should generated mesh use index buffer?
		//**********************************************************************
		virtual CRefObj<ISceneMarchingCubesElem> CreateMarchingCubesElem(IRenderer* pRenderer, uint32 subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed) = 0;

		//**********************************************************************
		// Method: CreateMeshElem
		// Creates a mesh element
		//**********************************************************************
		virtual CRefObj<ISceneMeshElem> CreateMeshElem() = 0;

		//**********************************************************************
		// Method: CreateLineElem
		// Creates a line element
		//**********************************************************************
		virtual CRefObj<ISceneLineElem> CreateLineElem(Vector3& p0, Vector3& p1) = 0;

		//**********************************************************************
		// Method: CreateSphereElem
		// Creates a sphere element
		//**********************************************************************
		virtual CRefObj<ISceneSphereElem> CreateSphereElem(Vector3& center, float radius) = 0;

		//**********************************************************************
		// Method: CreateCubeElem
		// Creates a cube element
		//**********************************************************************
		virtual CRefObj<ISceneCubeElem> CreateCubeElem(Vector3& center, float width, float height, float depth) = 0;

		//**********************************************************************
		// Method: CreateCylinderElem
		// Creates a mesh element
		//**********************************************************************
		virtual CRefObj<ISceneCylinderElem> CreateCylinderElem(Vector3 &center, float height, float topRadius, float bottomRadius) = 0;

		//**********************************************************************
		// Method: CreateMeshElem
		// Creates a mesh element
		//**********************************************************************
		virtual CRefObj<ISceneElem> CreateInstanceElem(ISceneElem* pTemplate) = 0;

		//**********************************************************************
		// Method: CreateCustomRenderElem
		// Create a element that will use a callback for rendering
		//**********************************************************************
		virtual CRefObj<ISceneCustomRenderElem> CreateCustomRenderElem(std::function<void(IRenderer* pRender, IRenderCtx* pCtx, SceneCtx* pSceneCtx)> clientCallback) = 0;

		//**********************************************************************
		// Method: CreateOverlay2DElem
		// Create a overlay element used to render textures in screen space (e.g. UI)
		//
		// Parameters:
		// pShader - override shader to use. If nullptr then default shader is used.
		//**********************************************************************
		virtual CRefObj<ISceneOverlay2DElem> CreateOverlay2DElem(IShader* pShader = nullptr) = 0;

		//**********************************************************************
		// Method: CreateSceneGraph
		// Creates a new scene graph
		//**********************************************************************
		virtual CRefObj<ISceneGraph> CreateSceneGraph() = 0;
	};

	//**********************************************************************
	// Function: CreateSceneFactory
	// Creates a scene factory. This factory is used to create elements in
	// the scene graph.
	//**********************************************************************
	CRefObj<ISceneFactory> CreateSceneFactory();
};
