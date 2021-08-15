//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Line.vs
//**********************************************************************
struct VSInput
{
    float3 pos : POSITION;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix worldViewProj;
    matrix endpoints;
};

VSOutput VS(VSInput p)
{
    VSOutput v;
    float4 wpos = mul(float4(p.pos, 1.0f), endpoints);
    v.pos = mul(wpos, worldViewProj);
    return v;
}