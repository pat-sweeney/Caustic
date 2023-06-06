//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Warp.vs
//**********************************************************************
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

VSOutput VS(VSInput p)
{
    VSOutput v;
    v.pos = float4(p.pos, 1.0f);
    v.uvs = p.uvs;
    return v;
}