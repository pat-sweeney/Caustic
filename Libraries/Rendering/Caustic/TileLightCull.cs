//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: TileLightCull.cs
//
// Compute shader for tiled forward light culling.
// Divides the screen into 16x16 pixel tiles and determines which
// lights affect each tile using the depth buffer for frustum culling.
//
// Output format:
// For each tile at (tileX, tileY):
//   tileIndex = tileY * numTilesX + tileX
//   tileLightData[tileIndex * (MAX_TILE_LIGHTS + 1) + 0] = light count
//   tileLightData[tileIndex * (MAX_TILE_LIGHTS + 1) + 1..N] = light indices
//**********************************************************************
#include "defs.h"

#define TILE_SIZE 16
#define MAX_TILE_LIGHTS 32

cbuffer ConstantBuffer
{
    matrix viewMatrix;
    matrix projMatrix;
    int numLights;
    int screenWidth;
    int screenHeight;
    float pad;
};

StructuredBuffer<LightData> lightBuffer : register(t0);
Texture2D<float> depthTexture : register(t1);

RWStructuredBuffer<uint> tileLightData : register(u0);

groupshared uint tileMinDepthInt;
groupshared uint tileMaxDepthInt;
groupshared uint tileLightCount;
groupshared uint tileLightIndices[MAX_TILE_LIGHTS];

[numthreads(TILE_SIZE, TILE_SIZE, 1)]
void CS(uint3 groupId : SV_GroupID,
        uint3 groupThreadId : SV_GroupThreadID,
        uint groupIndex : SV_GroupIndex,
        uint3 dispatchThreadId : SV_DispatchThreadID)
{
    // Initialize shared memory
    if (groupIndex == 0)
    {
        tileMinDepthInt = 0x7F7FFFFF; // FLT_MAX as uint
        tileMaxDepthInt = 0;
        tileLightCount = 0;
    }
    GroupMemoryBarrierWithGroupSync();

    // Read depth for this pixel and compute tile min/max depth
    float depth = 1.0;
    if (dispatchThreadId.x < (uint)screenWidth && dispatchThreadId.y < (uint)screenHeight)
    {
        depth = depthTexture.Load(int3(dispatchThreadId.xy, 0));
    }

    // Linearize depth: convert from [0,1] NDC to view-space Z
    float linearDepth = projMatrix._43 / (depth - projMatrix._33);

    uint depthInt = asuint(linearDepth);
    InterlockedMin(tileMinDepthInt, depthInt);
    InterlockedMax(tileMaxDepthInt, depthInt);
    GroupMemoryBarrierWithGroupSync();

    float tileMinDepth = asfloat(tileMinDepthInt);
    float tileMaxDepth = asfloat(tileMaxDepthInt);

    // Compute tile frustum planes in view space
    // Tile screen-space bounds
    float2 tileMin = float2(groupId.x * TILE_SIZE, groupId.y * TILE_SIZE);
    float2 tileMax = float2((groupId.x + 1) * TILE_SIZE, (groupId.y + 1) * TILE_SIZE);

    // Convert to NDC [-1, 1]
    float2 ndcMin = tileMin / float2(screenWidth, screenHeight) * 2.0 - 1.0;
    float2 ndcMax = tileMax / float2(screenWidth, screenHeight) * 2.0 - 1.0;
    ndcMin.y = -ndcMin.y;
    ndcMax.y = -ndcMax.y;

    // Frustum planes (view space): left, right, top, bottom, near, far
    // For a perspective projection, the frustum side planes pass through the origin
    float4 frustumPlanes[6];
    frustumPlanes[0] = float4( 1.0, 0.0, -ndcMin.x / projMatrix._11, 0.0); // left
    frustumPlanes[1] = float4(-1.0, 0.0,  ndcMax.x / projMatrix._11, 0.0); // right
    frustumPlanes[2] = float4(0.0,  1.0, -ndcMax.y / projMatrix._22, 0.0); // top (ndcMax.y is flipped)
    frustumPlanes[3] = float4(0.0, -1.0,  ndcMin.y / projMatrix._22, 0.0); // bottom
    frustumPlanes[4] = float4(0.0, 0.0,  1.0, -tileMinDepth);              // near
    frustumPlanes[5] = float4(0.0, 0.0, -1.0,  tileMaxDepth);              // far

    // Normalize side planes
    for (int p = 0; p < 4; p++)
    {
        frustumPlanes[p] /= length(frustumPlanes[p].xyz);
    }

    // Each thread tests a subset of lights
    for (uint lightIndex = groupIndex; lightIndex < (uint)numLights; lightIndex += TILE_SIZE * TILE_SIZE)
    {
        LightData light = lightBuffer[lightIndex];

        // Directional lights always affect all tiles
        if (light.type == LIGHT_TYPE_DIRECTIONAL)
        {
            uint offset;
            InterlockedAdd(tileLightCount, 1, offset);
            if (offset < MAX_TILE_LIGHTS)
                tileLightIndices[offset] = lightIndex;
            continue;
        }

        // Transform light position to view space
        float4 lightPosVS = mul(float4(light.posWS.xyz, 1.0), viewMatrix);
        float lightRadius = light.posWS.w; // range stored in w

        // Test sphere against each frustum plane
        bool inFrustum = true;
        for (int fp = 0; fp < 6; fp++)
        {
            float dist = dot(frustumPlanes[fp].xyz, lightPosVS.xyz) + frustumPlanes[fp].w;
            if (dist < -lightRadius)
            {
                inFrustum = false;
                break;
            }
        }

        if (inFrustum)
        {
            uint offset;
            InterlockedAdd(tileLightCount, 1, offset);
            if (offset < MAX_TILE_LIGHTS)
                tileLightIndices[offset] = lightIndex;
        }
    }
    GroupMemoryBarrierWithGroupSync();

    // Write results: first thread writes the tile's light list
    if (groupIndex == 0)
    {
        int numTilesX = (screenWidth + TILE_SIZE - 1) / TILE_SIZE;
        uint tileIndex = groupId.y * numTilesX + groupId.x;
        uint baseOffset = tileIndex * (MAX_TILE_LIGHTS + 1);

        uint lightCount = min(tileLightCount, MAX_TILE_LIGHTS);
        tileLightData[baseOffset] = lightCount;
        for (uint i = 0; i < lightCount; i++)
        {
            tileLightData[baseOffset + 1 + i] = tileLightIndices[i];
        }
    }
}
