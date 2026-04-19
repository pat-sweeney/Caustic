//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: ShadowMap.vs
//**********************************************************************
#include "morph.h"

struct VSInput
{
    float3 posOS : POSITION; // Vertex position in object coordinates
    float3 normOS : NORMAL; // Vertex normal in object coordinates
    float2 uvs : TEXCOORD0; // Texture coordinates
};

struct VSOutput
{
    float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 worldViewProj; // Model => Projected
    int morphTargetCount;
    int morphNumVertices;
    float2 morphPad;
    float4 morphWeights0;
    float4 morphWeights1;
};

VSOutput VS(VSInput p, uint vertexID : SV_VertexID)
{
    float3 pos = p.posOS;
    float3 norm = p.normOS;
    ApplyMorphTargets(vertexID, morphTargetCount, morphNumVertices,
        morphWeights0, morphWeights1, pos, norm);

    VSOutput v;
    v.posPS = mul(float4(pos, 1.0f), worldViewProj);
    return v;
}