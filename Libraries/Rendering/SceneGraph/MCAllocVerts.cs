//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
// File: MCAllocVerts.cs
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

globallycoherent RWStructuredBuffer<uint> cellMasks : register(u3);
globallycoherent RWStructuredBuffer<Counts> counts : register(u4);

[numthreads(8, 8, 8)]
void CS(uint3 DTid : SV_DispatchThreadID)
{
    uint numCellsXY = uint(numCellsX * numCellsY);
    uint addr = DTid.x + DTid.y * numCellsX + DTid.z * numCellsXY;

    int vertsToAlloc = 0;
    uint mask = cellMasks[addr];
    if (mask & 0x40000000)
        vertsToAlloc++;
    if (mask & 0x20000000)
        vertsToAlloc++;
    if (mask & 0x10000000)
        vertsToAlloc++;
    if (vertsToAlloc > 0)
    {
        int startIndex;
        InterlockedAdd(counts[0].numAllocatedVerts, vertsToAlloc, startIndex);
        InterlockedOr(cellMasks[addr], startIndex);
    }
}
