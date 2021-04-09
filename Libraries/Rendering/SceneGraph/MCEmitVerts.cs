//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
// File: MCEmitVerts.cs
//
// NOTE: This shader is part of a pipeline. The pipeline is expected
// to run all the following shaders in order:
//    MCCouuntVerts => MCAllocVerts => MCEmitVerts
//**********************************************************************

cbuffer commonstuff : register(b0)
{
    int numCells; // Number of voxels in each direction
};

struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};
static const int vertexSize = 32;

struct Counts
{
    uint numVertices; // Total number of vertices
    uint numIndices; // Total number of indices
    uint numAllocatedVerts; // Total number of vertices allocated thus far
    uint numEmittedIndices; // Total number of emitted indices
};

StructuredBuffer<float> densityField : register(t1); // Assumed to be normalized (0..1 x 0..1 x 0..1) and of size (numCells x numCells x numCells)
StructuredBuffer<uint> edgeTable : register(t2); // Assume to be of size [256][16] with format: [code]{ numPolys, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2 }
RWStructuredBuffer<uint> cellMasks : register(u3);
globallycoherent RWStructuredBuffer<Counts> counts : register(u4);
globallycoherent RWByteAddressBuffer indexBuffer : register(u5);
globallycoherent RWByteAddressBuffer vertexBuffer : register(u6);

// VertexToIndex converts a vertex index into a relative index
// in reference to the owning voxel.  For instance, in the following
// diagram, only vertices labeled 0, 3, and 8 are owned by the current
// voxel. All other vertices are owned by the neighboring voxels.
// The diagram is supposed to represent the surrounding voxels (without
// showing the neighboring voxels in the Z direction) and which voxels
// own which vertices.
//                                      +-----------------+      +-----------------+ 
//                                     /|                /|     /|                /| 
//                                    / |               / |    / |               / | 
//                                   /  |              /  |   /  |              /  | 
//                                  +-----------------+   |  +-----------------+   | 
//                                  |   |             |   |  |   |             |   | 
//                                  |   +-------------|---+  |   +-------------|---+ 
//                                  |  /              |  /   |  /              |  /  
//                                  | 9               | /    | 10              | /  
//                                  |/                |/     |/                |/    
//                                  +---------1-------+      +-----------------+     
//        +--------5--------+           +-----------------+      +-----------------+ 
//       /|                /|          /|                /|     /|                /| 
//      9 4              10 |         / |               / |    / |               / | 
//     /  |              /  6        /  |              /  |   /  |              /  | 
//    +----------1------+   |       +-----------------+   |  +-----------------+   | 
//    |   |             |   |   =>  |   |             |   |  |   |             |   | 
//    |   +--------7----|---+       |   +-------------|---+  |   +-------------|---+ 
//    0  /              2  /        0  /              |  /   2  /              |  /  
//    | 8               | 11        | 8               | /    | 11              | /  
//    |/                |/          |/                |/     |/                |/    
//    +---------3-------+           +-------3---------+      +-----------------+
//
// Each of these groups/voxel are then renumbered such that the vertices in the 0,3, and 8
// positions become 0, 1, 2. i.e. vertices 8, 9, 10, 11 all become 2. This renumbering matches
// the ordering that vertices are output to the vertex buffer.
uint VertexToIndex(uint va, int vi)
{
    // Not all vertices are referenced. Thus if only vertices 8 and 3 were referenced
    // our relative indices would become 1 and 0, whereas if all 3 vertices (0, 3, 8)
    // were referenced, our relative indices would become 0, 1, 2.
    // Using our cellMasks flags we can determine the correct mapping as:
    //     x000 = . . .
    //     x001 = . . 0
    //     x010 = . 0 .
    //     x011 = . 0 1
    //     x100 = 0 . .
    //     x101 = 0 . 1
    //     x110 = 0 1 .
    //     x111 = 0 1 2
    // where 'x' is the high bit on the cellMask, and . is a don't care state (since
    // vertex is unreferenced.
    int relOffset0 = 0;
    int relOffset1 = 0;
    int relOffset2 = 0;
    switch (cellMasks[va] & 0x7000000)
    {
        case 0x00000000: break;
        case 0x10000000: relOffset2 = 0; break;
        case 0x20000000: relOffset1 = 0; break;
        case 0x30000000: relOffset1 = 0; relOffset2 = 1; break;
        case 0x40000000: relOffset0 = 0; break;
        case 0x50000000: relOffset0 = 0; relOffset2 = 1; break;
        case 0x60000000: relOffset0 = 0; relOffset1 = 1; break;
        case 0x70000000: relOffset0 = 0; relOffset1 = 1; relOffset2 = 2; break;
    }
    int relativeID;
    switch (vi)
    {
        default:
        case 0: relativeID = relOffset0; break;
        case 1: relativeID = relOffset1; break;
        case 2: relativeID = relOffset0; break;
        case 3: relativeID = relOffset1; break;
        case 4: relativeID = relOffset0; break;
        case 5: relativeID = relOffset1; break;
        case 6: relativeID = relOffset0; break;
        case 7: relativeID = relOffset1; break;
        case 8: relativeID = relOffset2; break;
        case 9: relativeID = relOffset2; break;
        case 10: relativeID = relOffset2; break;
        case 11: relativeID = relOffset2; break;
    }
    int vbAddr = cellMasks[va] & 0xFFFFFF;
    return vbAddr + relativeID;
}

void EmitVertex(uint cellVal, uint mask, uint vbAddr, float3 edgePos)
{
    vertexBuffer.Store(vbAddr * vertexSize,      asuint(edgePos.x)); // vertex.x
    vertexBuffer.Store(vbAddr * vertexSize + 4,  asuint(edgePos.y)); // vertex.y
    vertexBuffer.Store(vbAddr * vertexSize + 8,  asuint(edgePos.z)); // vertex.z
    vertexBuffer.Store(vbAddr * vertexSize + 12, asuint(1.0f)); // vertex.nx
    vertexBuffer.Store(vbAddr * vertexSize + 16, asuint(1.0f)); // vertex.ny
    vertexBuffer.Store(vbAddr * vertexSize + 20, asuint(1.0f)); // vertex.nz
    vertexBuffer.Store(vbAddr * vertexSize + 24, asuint(0.0f)); // vertex.u
    vertexBuffer.Store(vbAddr * vertexSize + 28, asuint(0.0f)); // vertex.v
}

// VertexIDToVoxelAddr() converts a vertexId (0-12) for
// the specified voxel (addr) into the voxel address that
// has the info for that vertex. For instance, vertex 7 info
// will be stored as vertex 3 info in the voxel neighbor
// at +1 in Z (in our voxel grid)
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
uint VertexIDToVoxelAddr(uint addr, int vertexId)
{
    uint stepx = 0, stepy = 0, stepz = 0;
    switch (vertexId)
    {
        case 0: break;
        case 1: stepy = 1; break;
        case 2: stepx = 1; break;
        case 3: break;
        case 4: stepz = 1; break;
        case 5: stepy = 1; stepz = 1; break;
        case 6: stepx = 1; stepz = 1; break;
        case 7: stepz = 1; break;
        case 8: break;
        case 9: stepy = 1; break;
        case 10: stepx = 1; stepy = 1; break;
        case 11: stepx = 1; break;
    }
    // determine the address of the voxel that owns the vertex on edge e0.
    // Then set the flags on the owner voxel (see mask[] for description)
    // Finally increment our vertex count if the original value didn't have the use bit set (i.e. it didn't but now does, so we need to generate that vertex)
    return addr + stepx + stepy * numCells + stepz * numCells * numCells;
}

[numthreads(1, 1, 1)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    // Determine the address of this voxel
    uint numCells2 = numCells * numCells;
    uint addr = DTid.x + DTid.y * numCells + DTid.z * numCells2;
    uint cellVal = cellMasks[addr];

    // Compute the denisty at the voxels corners
    float density = densityField[addr];
    float delta = 1.0f / float(numCells);
    float d0 = densityField[addr];
    float d1 = densityField[addr + numCells];
    float d2 = densityField[addr + numCells + 1];
    float d3 = densityField[addr + 1];
    float d4 = densityField[addr + numCells2];
    float d5 = densityField[addr + numCells2 + numCells];
    float d6 = densityField[addr + numCells2 + numCells + 1];
    float d7 = densityField[addr + numCells2 + 1];

    // Compute configuration code
    uint code = 0;
    code = (code << 1) | ((d7 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d6 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d5 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d4 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d3 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d2 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d1 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d0 >= 0.0f) ? 1 : 0);
    if (code == 0 || code == 255)
        return;

    // Determine the zero crossing for each vertex
    float t0 = -d0 / (d1 - d0);
    float t3 = -d3 / (d0 - d3);
    float t8 = -d0 / (d4 - d0);

    // Find normalized position of the voxels
    float x = float(DTid.x) / float(numCells);
    float y = float(DTid.y) / float(numCells);
    float z = float(DTid.z) / float(numCells);
    float3 e0 = float3(x, y, z);
    float3 e1 = float3(x, y + delta, z);
    float3 e3 = float3(x + delta, y, z);
    float3 e4 = float3(x, y, z + delta);

    // Compute final vertices
    float3 edgePos0 = e0 + (e1 - e0) * t0;
    float3 edgePos3 = e3 + (e0 - e3) * t3;
    float3 edgePos8 = e0 + (e4 - e0) * t8;

    int vbAddr = cellVal & 0xFFFFFF;
    int offset = 0;
    if (cellVal & 0x40000000)
    {
        EmitVertex(cellVal, 0x40000000, vbAddr, edgePos0);
        offset++;
    }
    if (cellVal & 0x20000000)
    {
        EmitVertex(cellVal, 0x20000000, vbAddr + offset, edgePos3);
        offset++;
    }
    if (cellVal & 0x10000000)
    {
        EmitVertex(cellVal, 0x10000000, vbAddr + offset, edgePos8);
        offset++;
    }

    // Next reserve space for the indices in the index buffer
    int numPolys = edgeTable[code * 16];
    uint startIndex;
    InterlockedAdd(counts[0].numEmittedIndices, numPolys * 3, startIndex);
    startIndex *= 4;
    for (int l = 0; l < numPolys; l++)
    {
        uint i0 = edgeTable[code * 16 + 3 * l + 1];
        indexBuffer.Store(startIndex, VertexToIndex(VertexIDToVoxelAddr(addr, i0), i0));
        startIndex += 4;
        uint i1 = edgeTable[code * 16 + 3 * l + 2];
        indexBuffer.Store(startIndex, VertexToIndex(VertexIDToVoxelAddr(addr, i1), i1));
        startIndex += 4;
        uint i2 = edgeTable[code * 16 + 3 * l + 3];
        indexBuffer.Store(startIndex, VertexToIndex(VertexIDToVoxelAddr(addr, i2), i2));
        startIndex += 4;
    }
}
