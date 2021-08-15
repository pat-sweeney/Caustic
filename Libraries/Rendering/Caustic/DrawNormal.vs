//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
// File: DrawNormal.vs
//**********************************************************************
struct VSInput
{
    float3 pos : POSITION;
    float4 dir : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 worldView;
    float4x4 worldViewProj;
    float normalScale;
    float drawFaceNormals;
    float drawNormals;
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    float3 vpos = p.pos + p.dir.xyz * normalScale * lerp(drawFaceNormals, drawNormals, p.dir.w);
    v.pos = mul(float4(vpos, 1.0f), worldViewProj);
    return v;
}