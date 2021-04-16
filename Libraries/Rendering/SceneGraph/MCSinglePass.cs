//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
// File: MCSinglePass.cs
//**********************************************************************

cbuffer ConstantBuffer : register(b0)
{
    int subdivisions; // Original subdivisions requested by client
    int numCellsX; // Number of voxels in X direction
    int numCellsY; // Number of voxels in Y direction
    int numCellsZ; // Number of voxels in Z direction
    int countOnly; // Are we running this shader in the "countOnly" mode (where # verts are counted but not emitted)
};

struct Counts
{
    uint numVertices; // Total number of vertices
    uint numIndices; // Total number of indices
    uint numAllocatedVerts; // Total number of vertices allocated thus far
    uint numEmittedIndices; // Total number of emitted indices
};

struct Vertex
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};
static const int vertexSize = 32;

StructuredBuffer<float> densityField : register(t1); // Assumed to be normalized (0..1 x 0..1 x 0..1) and of size (numCells x numCells x numCells)
StructuredBuffer<int> edgeTable : register(t2); // Assume to be of size [256][16] with format: [code]{ numPolys, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2, i0, i1, i2 }
globallycoherent RWStructuredBuffer<Counts> counts : register(u4);
globallycoherent RWByteAddressBuffer vertexBuffer : register(u5);

void EmitVertex(uint vbAddr, float3 pos, float3 norm, float2 uv)
{
    vertexBuffer.Store(vbAddr * vertexSize, asuint(pos.x));
    vertexBuffer.Store(vbAddr * vertexSize + 4, asuint(pos.y));
    vertexBuffer.Store(vbAddr * vertexSize + 8, asuint(pos.z));
    vertexBuffer.Store(vbAddr * vertexSize + 12, asuint(norm.x));
    vertexBuffer.Store(vbAddr * vertexSize + 16, asuint(norm.y));
    vertexBuffer.Store(vbAddr * vertexSize + 20, asuint(norm.z));
    vertexBuffer.Store(vbAddr * vertexSize + 24, asuint(uv.x));
    vertexBuffer.Store(vbAddr * vertexSize + 28, asuint(uv.y));
}

[numthreads(8, 8, 8)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    uint numCellsXY = uint(numCellsX * numCellsY);
    uint addr = DTid.x + DTid.y * numCellsX + DTid.z * numCellsXY;
    float d0 = densityField[addr];
    float d1 = densityField[addr + numCellsX];
    float d2 = densityField[addr + numCellsX + 1];
    float d3 = densityField[addr + 1];
    float d4 = densityField[addr + numCellsXY];
    float d5 = densityField[addr + numCellsXY + numCellsX];
    float d6 = densityField[addr + numCellsXY + numCellsX + 1];
    float d7 = densityField[addr + numCellsXY + 1];
    float t0 = -d0 / (d1 - d0);
    float t1 = -d1 / (d2 - d1);
    float t2 = -d2 / (d3 - d2);
    float t3 = -d3 / (d0 - d3);
    float t4 = -d4 / (d5 - d4);
    float t5 = -d5 / (d6 - d5);
    float t6 = -d6 / (d7 - d6);
    float t7 = -d7 / (d4 - d7);
    float t8 = -d0 / (d4 - d0);
    float t9 = -d1 / (d5 - d1);
    float t10 = -d2 / (d6 - d2);
    float t11 = -d3 / (d7 - d3);
    float delta = 1.0f / float(subdivisions - 1);
    float x = float(DTid.x) / float(subdivisions - 1);
    float y = float(DTid.y) / float(subdivisions - 1);
    float z = float(DTid.z) / float(subdivisions - 1);
    float3 e0 = float3(x, y, z);
    float3 e1 = float3(x, y + delta, z);
    float3 e2 = float3(x + delta, y + delta, z);
    float3 e3 = float3(x + delta, y, z);
    float3 e4 = float3(x, y, z + delta);
    float3 e5 = float3(x, y + delta, z + delta);
    float3 e6 = float3(x + delta, y + delta, z + delta);
    float3 e7 = float3(x + delta, y, z + delta);
    float3 edgePos[12];
    edgePos[0] = e0 + (e1 - e0) * t0;
    edgePos[1] = e1 + (e2 - e1) * t1;
    edgePos[2] = e2 + (e3 - e2) * t2;
    edgePos[3] = e3 + (e0 - e3) * t3;
    edgePos[4] = e4 + (e5 - e4) * t4;
    edgePos[5] = e5 + (e6 - e5) * t5;
    edgePos[6] = e6 + (e7 - e6) * t6;
    edgePos[7] = e7 + (e4 - e7) * t7;
    edgePos[8] = e0 + (e4 - e0) * t8;
    edgePos[9] = e1 + (e5 - e1) * t9;
    edgePos[10] = e2 + (e6 - e2) * t10;
    edgePos[11] = e3 + (e7 - e3) * t11;
    uint code = 0;
    code = (code << 1) | ((d7 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d6 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d5 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d4 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d3 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d2 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d1 >= 0.0f) ? 1 : 0);
    code = (code << 1) | ((d0 >= 0.0f) ? 1 : 0);
    int numPolys = edgeTable[code * 16];
    uint startIndex;
    InterlockedAdd(counts[0].numVertices, numPolys * 3, startIndex);
    if (countOnly == 0)
    {
        for (int l = 0; l < numPolys; l++)
        {
            uint i0 = edgeTable[code * 16 + 3 * l + 1];
            uint i1 = edgeTable[code * 16 + 3 * l + 2];
            uint i2 = edgeTable[code * 16 + 3 * l + 3];
            float3 pos0 = edgePos[i0];
            float3 pos1 = edgePos[i1];
            float3 pos2 = edgePos[i2];
            float3 p0 = pos1 - pos0;
            float3 p1 = pos2 - pos0;
            float3 norm = normalize(cross(p0, p1));
            float2 uv = float2(0.0f, 0.0f);
            EmitVertex(startIndex++, pos0, norm, uv);
            EmitVertex(startIndex++, pos1, norm, uv);
            EmitVertex(startIndex++, pos2, norm, uv);
        }
    }
}
