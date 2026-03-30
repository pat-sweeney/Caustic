//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Default.vs
//**********************************************************************
#include "defs.h"

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
    float4x4 cascadeViewProj[NUM_CASCADES]; // World => cascade light space
    float4 lightPosWS;
};

VSOutput VS(VSInput p)
{
    VSOutput v;

    // Transform our vertex normal from object space to world space
    v.normWS = normalize(mul(float4(p.normOS,1.0f), worldInvTranspose).xyz);
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    v.viewDepth = v.posPS.w;
    [unroll]
    for (int i = 0; i < NUM_CASCADES; i++)
        v.cascadePosLS[i] = mul(float4(v.posWS, 1.0f), cascadeViewProj[i]);
    return v;
}
