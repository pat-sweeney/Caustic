//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
// File: MCCountVerts.cs
//
// NOTE: This shader is part of a pipeline. The pipeline is expected
// to run all the following shaders in order:
//    MCCouuntVerts => MCAllocVerts => MCEmitVerts
//**********************************************************************

cbuffer ConstantBuffer : register(b0)
{
    int subdivisions; // Original subdivisions requested by client
    int numCellsX; // Number of voxels in X direction
    int numCellsY; // Number of voxels in Y direction
    int numCellsZ; // Number of voxels in Z direction
};

struct Counts
{
    uint numVertices; // Total number of vertices
    uint numIndices; // Total number of indices
    uint numAllocatedVerts; // Total number of vertices allocated thus far
    uint numEmittedIndices; // Total number of emitted indices
};

StructuredBuffer<float> densityField : register(t1); // Assumed to be normalized (0..1 x 0..1 x 0..1) and of size (numCells x numCells x numCells)
StructuredBuffer<int> edgeTable : register(t2); // Assume to be of size [256][16] with format: [code]{ numPolys, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2 }
globallycoherent RWStructuredBuffer<uint> cellMasks : register(u3);
globallycoherent RWStructuredBuffer<Counts> counts : register(u4);

// Each voxel defines up to 12 potential vertices. Each voxel is only reponsible for creating vertices 0, 3, and 8.
// We rely on the neighboring voxels to create the rest of the vertices. Each voxel will keep 3 flags indicating
// whether a vertex (0, 3, or 8) are referenced by a triangle within the current or neighboring vertices. Each voxel
// also stores the index of where these vertices are stored in the vertex buffer. The stored index is the starting
// index with the vertices written in the order: 0, 3, 8.
// Below is a diagram showing the layout of each vertex for a given voxel.
//        +--------5--------+
//       /|                /|
//      9 4              10 |
//     /  |              /  6
//    +----------1------+   |
//    |   |             |   |
//    |   +--------7----|---+
//    0  /              2  /
//    | 8               | 11
//    |/                |/
//    +---------3-------+
static uint mask[12] =
{
    //                           +-------- Vertex 0 referenced
    //                           | +------ Vertex 3 referenced
    //                           | | +---- Vertex 8 referenced
    //                           | | |         +-- vertexBuffer offset
    //                           | | |         |
    //                           V V V         V
    0x40000000, // Vertex 0 :  . 1 . . . . . . <24b index>
    0x20000000, // Vertex 1 :  . . 1 . . . . . <24b index>
    0x40000000, // Vertex 2 :  . 1 . . . . . . <24b index>
    0x20000000, // Vertex 3 :  . . 1 . . . . . <24b index>
    0x40000000, // Vertex 4 :  . 1 . . . . . . <24b index>
    0x20000000, // Vertex 5 :  . . 1 . . . . . <24b index>
    0x40000000, // Vertex 6 :  . 1 . . . . . . <24b index>
    0x20000000, // Vertex 7 :  . . 1 . . . . . <24b index>
    0x10000000, // Vertex 8 :  . . . 1 . . . . <24b index>
    0x10000000, // Vertex 9 :  . . . 1 . . . . <24b index>
    0x10000000, // Vertex 10:  . . . 1 . . . . <24b index>
    0x10000000, // Vertex 11:  . . . 1 . . . . <24b index>
};

// VertexIDToVoxelAddr() converts a vertexId (0-12) for
// the specified voxel (addr) into the voxel address that
// has the info for that vertex. For instance, vertex 7 info
// will be stored as vertex 3 info in the voxel neighbor
// at +1 in Z (in our voxel grid)
uint VertexIDToVoxelAddr(uint addr, int vertexId)
{
    uint stepx = 0, stepy = 0, stepz = 0;
    switch (vertexId)
    {
        case 0: break;
        case 1: stepy = 1; break;
        case 2: stepx = 1; break;
        case 3: break;
        case 4:            stepz = 1; break;
        case 5: stepy = 1; stepz = 1; break;
        case 6: stepx = 1; stepz = 1; break;
        case 7:            stepz = 1; break;
        case 8: break;
        case 9:             stepy = 1; break;
        case 10: stepx = 1; stepy = 1; break;
        case 11: stepx = 1; break;
    }
    // determine the address of the voxel that owns the vertex on edge e0.
    // Then set the flags on the owner voxel (see mask[] for description)
    // Finally increment our vertex count if the original value didn't have the use bit set (i.e. it didn't but now does, so we need to generate that vertex)
    return addr + stepx + stepy * numCellsX + stepz * numCellsX * numCellsY;
}

[numthreads(8, 8, 8)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= (uint)subdivisions || DTid.y >= (uint)subdivisions || DTid.z >= (uint)subdivisions)
        return;
    uint numCellsXY = uint(numCellsX * numCellsY);
    uint addr = DTid.x + DTid.y * numCellsX + DTid.z * numCellsXY;

    // Determine the signed distance value at each of the 8 corners of our voxel
    float d0 = densityField[addr];
    float d1 = densityField[addr + numCellsX];
    float d2 = densityField[addr + numCellsX + 1];
    float d3 = densityField[addr + 1];
    float d4 = densityField[addr + numCellsXY];
    float d5 = densityField[addr + numCellsXY + numCellsX];
    float d6 = densityField[addr + numCellsXY + numCellsX + 1];
    float d7 = densityField[addr + numCellsXY + 1];

    // From the density values determine the code defining the cube configuration
    uint code = 0;
    code = (code << 1) | ((d7 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d6 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d5 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d4 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d3 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d2 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d1 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d0 >= 0.0f) ? 1 : 0);

    // Given our configuration code we can now determine which vertices form polygons
    // in this voxel and set the usage flags (mask) for these in cellMask.
    uint totalVertices = 0;
    int numPolys = edgeTable[code * 16];
    for (int l = 0; l < numPolys; l++)
    {
        int e0 = edgeTable[code * 16 + 3 * l + 1];
        int e1 = edgeTable[code * 16 + 3 * l + 2];
        int e2 = edgeTable[code * 16 + 3 * l + 3];

        // determine the address of the voxel that owns the vertex on edge e0.
        // Then set the flags on the owner voxel (see mask[] for description)
        // Finally increment our vertex count if the original value didn't have the use bit set (i.e. it didn't but now does, so we need to generate that vertex)
        uint e0Addr = VertexIDToVoxelAddr(addr, e0);
        uint origFlag0 = 0;
        InterlockedOr(cellMasks[e0Addr], mask[e0], origFlag0);
        uint f0 = (origFlag0 & mask[e0]);
        totalVertices += (f0 == 0) ? 1 : 0;

        // Repeat for vertex e1
        uint e1Addr = VertexIDToVoxelAddr(addr, e1);
        uint origFlag1 = 0;
        InterlockedOr(cellMasks[e1Addr], mask[e1], origFlag1);
        uint f1 = (origFlag1 & mask[e1]);
        totalVertices += (f1 == 0) ? 1 : 0;

        // Repeat for vertex e2
        uint e2Addr = VertexIDToVoxelAddr(addr, e2);
        uint origFlag2 = 0;
        InterlockedOr(cellMasks[e2Addr], mask[e2], origFlag2);
        uint f2 = (origFlag2 & mask[e2]);
        totalVertices += (f2 == 0) ? 1 : 0;
    }

    // At this point we have marked all the vertices of all triangles in this voxel
    // as having been referenced. We next need to determine where in the vertexBuffer
    // triangles from this voxel (but not our neighbors) will be stored. 
    // Mark our cellMasks with where the vertices will be stored in the vertex buffer
    InterlockedAdd(counts[0].numVertices, totalVertices);
    InterlockedAdd(counts[0].numIndices, numPolys * 3);
}
