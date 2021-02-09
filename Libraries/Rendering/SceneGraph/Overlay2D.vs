//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Overlay2D.vs
//**********************************************************************
struct VSInput
{
    float4 posOS : POSITION; // Vertex position in object coordinates
    float2 uvs : TEXCOORD0; // Texture coordinates
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float2 scale;
    float2 offset;
};

struct VSOutput
{
    float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
    float2 uvs : TEXCOORD0; // UV coordinates
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    v.posPS = float4((p.posOS.xy * scale + offset) * 2.0f - 1.0f, 0.1f, 1.0f);
    v.uvs = p.uvs;
    return v;
}