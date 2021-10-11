//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base/Math/BBox.h"
#include "IRenderMesh.h"
#include "Shader.h"
#include <d3d11.h>

namespace Caustic
{
    //**********************************************************************
    // Function: BuildVertexBuffer
    // Internal function used to create a vertex buffer from a list of vertices
    //
    // Parameters:
    // pRenderer - The renderer
    // pShaderInfo - Information about the shader (need to get the vertex declaration)
    // pts - list of vertices
    // pMeshData - created vertex buffer is stored in this structure
    //
	// Header:
	// [Link:Rendering/Caustic/ConstructBuffer.h]
    //**********************************************************************
    extern void BuildVertexBuffer(IRenderer* pRenderer, IShaderInfo* pShaderInfo, std::vector<CGeomVertex>& pts, MeshData* pMeshData);
	
    //**********************************************************************
    // Function: BuildIndexBuffer
    // Internal function used to create an index buffer from a list of faces
    //
    // Parameters:
    // pRenderer - The renderer
    // faces - list of faces
    // pMeshData - created index buffer is stored in this structure
    //
    // Header:
    // [Link:Rendering/Caustic/ConstructBuffer.h]
    //**********************************************************************
    extern void BuildIndexBuffer(IRenderer* pRenderer, std::vector<uint32>& faces, MeshData* pMeshData);
}
