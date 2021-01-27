//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// All Rights Reserved
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

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float minu;
    float minv;
    float maxu;
    float maxv;
};

VSOutput VS(VSInput p)
{
    VSOutput v;

    float nx = (p.pos.x + 1.0f) / 2.0f;
    float ny = (p.pos.y + 1.0f) / 2.0f;
    nx = nx * (maxu - minu) + minu;
    ny = ny * (maxv - minv) + minv;
    nx = nx * 2.0f - 1.0f;
    ny = ny * 2.0f - 1.0f;
    v.pos = float4(nx, ny, p.pos.z, 1.0f);
    v.uvs = p.uvs;
    return v;
}