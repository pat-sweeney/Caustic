//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "defs.h"

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    return v;
}