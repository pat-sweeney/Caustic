//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Warp.vs
//**********************************************************************
Texture2D<float2> posTexture : register(t0);
SamplerState posSampler : register(s0);
struct VSInput
{
    float3 pos : POSITION;
    float2 uvs : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uvs : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float width;
    float height;
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    int3 pc = int3(p.uvs.x * width, p.uvs.y * height, 0);
    float2 delta = posTexture.Load(pc);
    v.pos = float4(p.pos.x + delta.x, -(p.pos.y + delta.y), 0.0f, 1.0f);
    v.uvs = p.uvs;
    return v;
}
