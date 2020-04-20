//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
struct Vertex
{
    int4 pos;
    int4 norm;
};

cbuffer ConstantBuffer
{
    int imageWidth;
    int imageHeight;
};

RWByteAddressBuffer DepthBuffer : register(u0);
RWByteAddressBuffer Points : register(u1);

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID )
{
    uint byteaddr = 2 * DTid.x + DTid.y * imageWidth * 2;
    byteaddr = (byteaddr / 4) *4;
    uint lword = DepthBuffer.Load(byteaddr);
    Points.Store(byteaddr, lword);
}
