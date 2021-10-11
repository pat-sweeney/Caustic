//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// All Rights Reserved
// File: Quad.vs
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