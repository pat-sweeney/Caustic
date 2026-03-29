//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Obj.vs
//**********************************************************************
#define MAX_LIGHTS 4
#define NUM_CASCADES 4

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
    float viewDepth : TEXCOORD3; // View-space depth for cascade selection
    float4 cascadePosLS[NUM_CASCADES] : TEXCOORD4; // Position in each cascade's light space
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
    float4x4 cascadeViewProj[NUM_CASCADES]; // World => cascade light space
    float4 lightPosWS;
};

VSOutput VS(VSInput p)
{
    VSOutput v;

    // Transform our vertex normal from object space to world space
    v.normWS = normalize(mul(float4(p.normOS,1.0f), worldInvTranspose).xyz);
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    v.posPS = mul(float4(p.posOS, 1.0f), worldViewProj);

    // Compute view-space depth for cascade selection
    float4x4 view = viewInv; // viewInv is world->view? Actually it's view->world.
    // We can get view-space Z from the clip-space position
    v.viewDepth = v.posPS.w; // In LH perspective, w = view-space Z

    // Transform world position into each cascade's light space
    [unroll]
    for (int i = 0; i < NUM_CASCADES; i++)
        v.cascadePosLS[i] = mul(float4(v.posWS, 1.0f), cascadeViewProj[i]);

    v.uvs = p.uvs;
    return v;
}