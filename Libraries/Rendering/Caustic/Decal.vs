//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Decal.vs
//
// Vertex shader for projected decals. Transforms a unit cube
// (vertices at +/-0.5 on each axis) to world space via the decal's
// OBB transform, then to clip space via the camera's view-projection.
//**********************************************************************

struct VSInput
{
    float3 posOS : POSITION;
};

struct VSOutput
{
    float4 posPS : SV_POSITION;
    float4 posCS : TEXCOORD0;   // clip-space position for depth reconstruction
};

cbuffer VS_CONSTANT_BUFFER
{
    matrix worldViewProj;       // decalWorld * view * proj
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    output.posPS = mul(float4(input.posOS, 1.0), worldViewProj);
    output.posCS = output.posPS;
    return output;
}
