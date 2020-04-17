//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
struct Vertex
{
    float4 pos;
    float4 norm;
};

cbuffer ConstantBuffer
{
    int imageWidth;
    int imageHeight;
};

StructuredBuffer<min16uint> DepthBuffer : register(t2);
RWByteAddressBuffer Points : register(u1);

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID )
{
    uint depth = DepthBuffer[DTid.x + DTid.y * imageWidth];
    uint addr = DTid.x + DTid.y * imageWidth;
    float4 pos = float4(float(depth),0.0,0.0f,0.0f);
    Points.Store4(addr, pos);
    float4 norm = float4(float(depth),0.0f,0.0f,0.0f);
    Points.Store4(addr, norm);
}
