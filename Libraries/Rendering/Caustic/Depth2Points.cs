//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
cbuffer ConstantBuffer
{
    int imageWidth;
    int imageHeight;
};

StructuredBuffer<min16uint> DepthBuffer : register(t0); // assumed to be 16bpp
StructuredBuffer<float3> RayBuffer : register(t1); // Defines undistorted ray from camera
RWStructuredBuffer<float3> Points : register(u2);

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID )
{
    uint addr = DTid.x + DTid.y * imageWidth;
    uint depth = DepthBuffer[addr];
    uint ldepth = depth >> 16;
    uint rdepth = depth & 0xffff;
    float3 dir = RayBuffer[addr];
    Points[addr] = dir * float(depth) / 1000.0f;
}
