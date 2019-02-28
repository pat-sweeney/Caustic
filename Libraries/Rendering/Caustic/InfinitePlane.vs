//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
struct VSInput
{
    float4 posOS : POSITION; // Vertex position in object coordinates
};

struct VSOutput
{
    float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
    float4 posPS2 : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 worldViewProj; // Model => Projected
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    v.posPS = mul(p.posOS, worldViewProj);
    v.posPS2 = v.posPS;
    return v;
}