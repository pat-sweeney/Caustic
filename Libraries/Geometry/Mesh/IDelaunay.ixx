//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <cinttypes>

export module Geometry.Mesh.IDelaunay;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.BBox;

//**********************************************************************
// File: Delaunay.ixx
// Header with class/methods for performing Delaunay triangulation
//**********************************************************************

export namespace Caustic
{
    const uint8_t c_SuperTriangleVertex = 0x1;
    const uint8_t c_BoundaryVertex = 0x2;
    const uint8_t c_InteriorVertex = 0x4;
    const uint8_t c_FixedVertex = 0x8;

    struct DelaunayVertex
    {
        uint8_t flags;
        Vector2 pos;
        Vector2 uv;

        DelaunayVertex() : flags(0)
        {
        }

        DelaunayVertex(Vector2& position, Vector2& uvCoord, uint8_t vertexFlag)
        {
            flags = vertexFlag;
            pos = position;
            uv = uvCoord;
        }
    };

    //**********************************************************************
    // Interface: IDelaunay2
    // Defines the interface used for creating Delaunay triangulations
    // from a set of 2D points.
    //**********************************************************************
    struct IDelaunay2 : public IRefCount
    {
        //**********************************************************************
        // Method: Open
        // Opens the triangulation so points can be added
        //**********************************************************************
        virtual void Open() = 0;

        //**********************************************************************
        // Method: AddPoint
        // Opens the triangulation so points can be added
        //
        // Parameters:
        // pt - point to add to triangulation
        // uv - UV coordinates of the point
        // isBoundary - True if point is part of the triangulation boundary
        //**********************************************************************
        virtual void AddPoint(Vector2 &pt, Vector2 &uv, bool isBoundary) = 0;

        //**********************************************************************
        // Method: AddFixedMesh
        // Adds mesh connectivity that should be fixed on the final mesh
        //**********************************************************************
        virtual void AddFixedMesh(DelaunayVertex* pVertices, int numVertices, int* pIndices, int numIndices) = 0;
        
        //**********************************************************************
        // Method: Close
        // Closes the triangulation and performs the Delaunay triangulation on
        // the set of points
        // 
        // Parameters:
        // removeSuperTriangles - remove super triangles?
        //**********************************************************************
        virtual void Close(bool removeSuperTriangles) = 0;

        //**********************************************************************
        // Method: GetNumberTriangles
        // Returns the number of triangles in the triangulation
        //**********************************************************************
        virtual uint32_t GetNumberTriangles() = 0;

        //**********************************************************************
        // Method: GetTriangle
        // Returns the Nth triangle
        //
        // Parameters:
        // index - index of triangle to be retrieved
        // v0 - first point in triangle
        // v1 - second point in triangle
        // v2 - third point in triangle
        // isExterior - flags indicating whether the returned points are part of the
        // triangulations boundary polygon
        //**********************************************************************
        virtual void GetTriangle(uint32_t index, Vector2 &v0, Vector2 &v1, Vector2 &v2, bool isExterior[3]) = 0;

        //**********************************************************************
        // Method: GetTriangleIndices
        // Returns the indices of the Nth triangle
        //
        // Parameters:
        // index - index of triangle to be retrieved
        // i0 - index of the first point in triangle
        // i1 - index of the second point in triangle
        // i2 - index of the third point in triangle
        // isExterior - flags indicating whether the returned points are part of the
        // triangulations boundary polygon
        //**********************************************************************
        virtual void GetTriangleIndices(uint32_t index, uint32_t& i0, uint32_t& i1, uint32_t& i2, bool isExterior[3]) = 0;

        //**********************************************************************
        // Method: WritePLY
        // Helper function for writing the triangulation out as a .PLY file
        //**********************************************************************
        virtual void WritePLY() = 0;
    };

    CRefObj<IDelaunay2> CreateDelaunay2(BBox2 &bb);
    CRefObj<IDelaunay2> CreateDelaunay2(DelaunayVertex* pVertices, int numVertices, int* pIndices, int numIndices);
}
