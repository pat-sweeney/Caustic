//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
// File: Depth2Points.cs
//**********************************************************************
cbuffer ConstantBuffer
{
    int depthImageWidth; // Width of depth image / 2. Half the size since reads are 32bpp chunks instead of 16bpp
};

struct Vector
{
    float3 pos;
    float3 norm;
};

StructuredBuffer<uint> DepthBuffer : register(t0); // assumed to be 16bpp, but read as 32bpp (thus image is half its normal size)
StructuredBuffer<float4> RayBuffer : register(t1); // Defines undistorted ray from camera
RWStructuredBuffer<Vector> Points : register(u2);

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID )
{
    // First read the depth value. We read two depth values since we can only read 32bpp chunks instead of 16bpp.
    // Thus we will process two points at once. Due to this 'depthImageWidth' is half the width of the actual depth map.
    uint addr = DTid.x + DTid.y * depthImageWidth;
    uint depth = DepthBuffer[addr];
    uint ldepth = depth >> 16;
    uint rdepth = depth & 0xffff;

    // Next read our direction ray. This map and our output 'Points' buffer are sized to match
    // the depth map (i.e. the real depth map, not the half sized thing we get passed)
    uint fulladdr = 2 * addr;
    float4 ldir = RayBuffer[fulladdr];
    Points[fulladdr].pos = ldir.xyz * float(ldepth) / 1000.0f;
    Points[fulladdr].norm = ldir.xyz;
    fulladdr += 1;
    float4 rdir = RayBuffer[fulladdr];
    Points[fulladdr].pos = rdir.xyz * float(rdepth) / 1000.0f;
    Points[fulladdr].norm = rdir.xyz;
}
