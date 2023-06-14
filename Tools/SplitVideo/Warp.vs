//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Warp.vs
//**********************************************************************
Texture2D<float2> posTexture;
SamplerState posSampler : register(s0);
struct VSInput
{
    float3 pos : POSITION;
    float2 uvs : TEXCOORD0;
    float2 coord : TEXCOORD1;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 uvs : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float width;
    float height;
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    uint2 pos_xy = { (uint)(p.coord.x * 100.0f), 0};
    float2 delta = posTexture[pos_xy];
//    int3 pc = int3((int)(p.coord.x * 100.0f), 0, 0);
//    float2 delta = posTexture.Load(pc).xy;
    v.pos = float4(p.pos.x + delta.x, -(p.pos.y + delta.y), 0.0f, 1.0f);
    v.uvs.xy = p.uvs;
    v.uvs.z = (delta.x > 0 || delta.y >0) ? 1 : 0;
    return v;
}
