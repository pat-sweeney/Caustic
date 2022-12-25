//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>

export module Rendering.Caustic.ConstructBuffer;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderMesh;
import Rendering.Caustic.IShaderInfo;
import Rendering.Caustic.IRenderer;

export namespace Caustic
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
    // Module:
    // {Link:import Rendering.Caustic.ConstructBuffer;{Rendering/Caustic/ConstructBuffer.ixx}}
    //**********************************************************************
    void BuildVertexBuffer(IRenderer* pRenderer, IShaderInfo* pShaderInfo, std::vector<CGeomVertex>& pts, MeshData* pMeshData);
    
    //**********************************************************************
    // Function: BuildIndexBuffer
    // Internal function used to create an index buffer from a list of faces
    //
    // Parameters:
    // pRenderer - The renderer
    // faces - list of faces
    // pMeshData - created index buffer is stored in this structure
    //
    // Module:
    // {Link:import Rendering.Caustic.ConstructBuffer;{Rendering/Caustic/ConstructBuffer.ixx}}
    //**********************************************************************
    void BuildIndexBuffer(IRenderer* pRenderer, std::vector<uint32>& faces, MeshData* pMeshData);
}
