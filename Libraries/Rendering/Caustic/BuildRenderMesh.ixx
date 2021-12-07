//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <string>
#include <atlbase.h>
#include <memory>
#include <vector>

export module Rendering.Caustic.BuildRenderMesh;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.RenderTypes;
import Rendering.Caustic.IRenderer;

export namespace Caustic
{
	//**********************************************************************
	// Method: BuildIndexBuffer
	// Creates a index buffer using the specified vertex and face lists.
	//
	// Parameters:
	// pRenderer - Graphics device
	// faces - list of faces
	// verts - list of vertices
	// vertexReferenced - List indicating which vertices were referenced by the face list
	// MeshData - place to store created index buffer
	//**********************************************************************
	void BuildIndexBuffer(IRenderer* pRenderer, std::vector<CGeomFace>& faces,
		std::vector<CGeomVertex>& verts,
		std::vector<int>& vertexReferenced, MeshData* pMeshData);

	//**********************************************************************
	// Method: BuildRenderSubMesh
	// Converts a list of faces and vertices into a IRenderSubMesh.
	//
	// Parameters:
	// pRenderer - Renderer
	// faces - list of faces
	// verts - list of vertices
	// pShader - shader
	//
	// Returns:
	// Returns the new render submesh
	//**********************************************************************
	CRefObj<IRenderSubMesh> BuildRenderSubMesh(IRenderer* pRenderer,
		std::vector<CGeomFace>& faces, std::vector<CGeomVertex>& verts,
		IShader* pShader);

	//**********************************************************************
	// Method: CreateDepthGridMesh
	// Creates a render submesh that is a grid. This is used for rendering
	// a depth map as a mesh.
	//
	// Parameters:
	// pRenderer - Renderer
	// width - width of depth map
	// height - height of depth map
	// pShader - shader
	//
	// Returns:
	// Returns the new render submesh
	//**********************************************************************
	CRefObj<IRenderSubMesh> CreateDepthGridMesh(IRenderer* pRenderer, uint32 width, uint32 height, IShader* pShader);
};
