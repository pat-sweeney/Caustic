//**********************************************************************
// Copyright Patrick Sweeney 2015-2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#define MAX_LIGHTS 16
#define NUM_CASCADES 4

// Light type constants (matches ELightType enum in C++)
#define LIGHT_TYPE_POINT       0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_SPOT        2
#define LIGHT_TYPE_AREA        3

struct LightData
{
    float4 posWS;       // xyz = position, w = range
    float4 dirWS;       // xyz = direction (for directional/spot), w = unused
    float4 color;       // rgb = color, a = unused
    float intensity;
    int type;           // LIGHT_TYPE_*
    int shadowIndex;    // -1 = no shadow, 0+ = shadow map index
    float pad;
};

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
