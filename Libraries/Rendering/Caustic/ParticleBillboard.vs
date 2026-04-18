//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: ParticleBillboard.vs
//
// Vertex shader for GPU particle billboard rendering. Reads particle
// data from a StructuredBuffer via SV_InstanceID and expands quad
// corners into camera-facing billboards. Dead particles (age < 0)
// produce degenerate zero-size triangles.
//**********************************************************************

struct Particle
{
    float3 pos;
    float  age;
    float3 vel;
    float  lifetime;
    float  size;
    float  pad;
    float4 color;
};

StructuredBuffer<Particle> particleBuffer : register(t0);

struct VSInput
{
    float2 corner : POSITION;        // [-0.5, +0.5] quad corners
    uint   instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 posPS  : SV_POSITION;
    float4 color  : COLOR0;
    float2 uvs    : TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER
{
    matrix viewMatrix;
    matrix projMatrix;
    float3 camRight;
    float  pad1;
    float3 camUp;
    float  pad2;
};

VSOutput VS(VSInput input)
{
    VSOutput output;

    Particle p = particleBuffer[input.instanceID];

    // Dead particles: output degenerate triangle
    if (p.age < 0.0)
    {
        output.posPS = float4(0.0, 0.0, 0.0, 0.0);
        output.color = float4(0.0, 0.0, 0.0, 0.0);
        output.uvs = float2(0.0, 0.0);
        return output;
    }

    // Expand billboard in world space
    float3 worldPos = p.pos
        + camRight * input.corner.x * p.size
        + camUp * input.corner.y * p.size;

    // Transform to clip space
    float4 viewPos = mul(float4(worldPos, 1.0), viewMatrix);
    output.posPS = mul(viewPos, projMatrix);

    output.color = p.color;
    output.uvs = input.corner + 0.5; // [0,1] range for texture sampling

    return output;
}
