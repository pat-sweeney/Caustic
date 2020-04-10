//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// All Rights Reserved
//**********************************************************************

Texture2D<uint> depthTexture : register(t0);
SamplerState depthSampler : register(s0);

#define MAX_LIGHTS 4

struct VSInput
{
    float3 posOS : POSITION; // Vertex position in object coordinates
    float3 normOS : NORMAL; // Vertex normal in object coordinates
    float2 uvs : TEXCOORD0; // Texture coordinates
};

struct VSOutput
{
    float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
    float3 posWS : TEXCOORD0;
    float3 normWS : TEXCOORD1; // Normal vector in world coordinates
    float2 uvs : TEXCOORD2; // UV coordinates
    float depth : TEXCOORD3;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
    float4 lightPosWS;
};

VSOutput VS(VSInput p)
{
    VSOutput v;

    int3 pc = int3(p.uvs.x * 512, p.uvs.y * 512, 0);
    v.depth = depthTexture.Load(pc);
    // Transform our vertex normal from object space to world space
    v.normWS = normalize(mul(float4(p.normOS,1.0f), worldInvTranspose).xyz);
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    return v;
}