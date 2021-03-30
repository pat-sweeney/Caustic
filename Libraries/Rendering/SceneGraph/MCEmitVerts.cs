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

cbuffer ConstantBuffer
{
    int numCells; // Number of voxels in each direction
};

struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

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
RWStructuredBuffer<Counts> counts : register(u4);
RWStructuredBuffer<uint> indexBuffer : register(u5);
RWStructuredBuffer<Vertex> vertexBuffer : register(u6);

uint VertexToIndex(uint addr, int vi)
{
    int relativeID;
    switch (vi)
    {
        case 0: relativeID = 0; break;
        case 1: relativeID = 1; break;
        case 2: relativeID = 0; break;
        case 3: relativeID = 1; break;
        case 4: relativeID = 0; break;
        case 5: relativeID = 1; break;
        case 6: relativeID = 0; break;
        case 7: relativeID = 1; break;
        case 8: relativeID = 2; break;
        case 9: relativeID = 2; break;
        case 10: relativeID = 2; break;
        case 11: relativeID = 2; break;
    }
    int vbAddr = cellMasks[addr] & 0xFFFFFF;
    return vbAddr + relativeID;
}

void EmitVertex(uint addr, uint mask, uint vbAddr, float3 edgePos)
{
    if (cellMasks[addr] & mask)
    {
        Vertex v;
        v.nx = 0.0f;
        v.ny = 0.0f;
        v.nz = 0.0f;
        v.u = 0.0f;
        v.v = 0.0f;
        v.x = edgePos.x;
        v.y = edgePos.y;
        v.z = edgePos.z;
        vertexBuffer[vbAddr] = v;
    }
}

[numthreads(8, 8, 8)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    // Determine the address of this voxel
    uint numCells2 = numCells * numCells;
    uint addr = DTid.x + DTid.y * numCells + DTid.z * numCells2;

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

    int vbAddr = cellMasks[addr] & 0xFFFFFF;
    EmitVertex(addr, 0x40000000, vbAddr, edgePos0);
    EmitVertex(addr, 0x20000000, vbAddr + 1, edgePos3);
    EmitVertex(addr, 0x10000000, vbAddr + 2, edgePos8);

    // Next reserve space for the indices in the index buffer
    int numPolys = edgeTable[code * 16];
    int startIndex;
    InterlockedAdd(counts[0].numEmittedIndices, numPolys * 3, startIndex);
    for (int l = 0; l < numPolys; l++)
    {
        indexBuffer[startIndex++] = VertexToIndex(addr, edgeTable[code * 16 + 3 * l + 0]);
        indexBuffer[startIndex++] = VertexToIndex(addr, edgeTable[code * 16 + 3 * l + 1]);
        indexBuffer[startIndex++] = VertexToIndex(addr, edgeTable[code * 16 + 3 * l + 2]);
    }
}
