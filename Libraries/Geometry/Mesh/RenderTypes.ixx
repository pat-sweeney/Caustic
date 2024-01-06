//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <d3d11.h>
#include <string>

export module Geometry.Mesh.RenderTypes;
import Base.Core.Core;
import Base.Math.Vector;
import Base.Math.BBox;

export namespace Caustic
{
    //**********************************************************************
    // Struct: CGeomVertex
    // Defines a vertex in our mesh. 
    //
    // Defines a vertex on our mesh. We have two types of vertex data:
    // 1) data that is specific to a vertex relative to a given face
    // 2) data that is relative to a vertex regardless of face
    // The reason for this split is so that positional information
    // can be shared across the mesh (useful for performing operations
    // on the geometry) while data that can change from face to face
    // is stored separately (such as normal information).
    // Typically most mesh data is stored in CGeomVertex. Only faces
    // at material boundaries or smoothing groups contain CFaceVertex data.
    //
    // Module:
    // {Link: import Rendering.Caustic.RenderTypes;{Rendering/Caustic/RenderTypes.ixx}}
    //**********************************************************************
    struct CGeomVertex
    {
        Vector3 pos;        // Defines the position
        Vector3 norm;       // Defines the normal
        Vector2 uvs[4];     // Defines UV coordinates
        int index;          // Index (used for serialization)
    };

    //**********************************************************************
    // Struct: CGeomFace
    // Defines a face (triangle) on our mesh as a tuple of vertex indices.
    //**********************************************************************
    struct CGeomFace
    {
        int indices[3];
    };

    //**********************************************************************
    // Struct: MeshData
    // Defines class for holding mesh data (i.e. vertices and indices)
    //
    // Members:
    // m_spVB - The vertex buffer
    // m_spIB - The index buffer (maybe null)
    // m_vertexSize - Size in bytes of each vertex
    // m_numVertices - Number of vertices
    // m_numIndices - Number of indices. Maybe 0
    //
    // Module:
    // {Link:import Geometry.Mesh.RenderTypes"{Geometry/Mesh/RenderTypes.ixx}}
    //**********************************************************************
    struct MeshData
    {
        CComPtr<ID3D11Buffer> m_spVB;
        CComPtr<ID3D11Buffer> m_spIB;
        uint32_t m_vertexSize;
        uint32_t m_numVertices;
        uint32_t m_numIndices;
        BBox3 m_bbox;
        std::string m_name;

        MeshData() :
            m_vertexSize(0),
            m_numVertices(0),
            m_numIndices(0)
        {
        }
    };
}
