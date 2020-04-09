//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// All Rights Reserved
//**********************************************************************
#include "defs.h"

Texture2D depthTexture : register(t0);
Texture2D rayTexture : register(t1);
SamplerState depthSampler : register(s0);

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
    float4 lightPosWS;
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    float depth = depthTexture.Sample(depthSampler, p.uvs);
    float2 ray = rayTexture.Sample(depthSampler, p.uvs);
    float3 pos = float3(ray.x * depth, ray.y * depth, depth);
    v.normWS = normalize(mul(float4(p.normOS,1.0f), worldInvTranspose).xyz);
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    return v;
}
