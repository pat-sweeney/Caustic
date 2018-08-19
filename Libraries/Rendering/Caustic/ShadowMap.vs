//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
struct VSInput
{
    float3 posOS : POSITION; // Vertex position in object coordinates
    float3 normOS : NORMAL; // Vertex normal in object coordinates
    float2 uvs : TEXCOORD0; // Texture coordinates
};

struct VSOutput
{
    float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
    float depth : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 worldViewProj; // Model => Projected
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);
    return v;
}