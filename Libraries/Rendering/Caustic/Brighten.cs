//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
cbuffer ConstantBuffer
{
    int imageWidth;
    int imageHeight;
    int brightness;
};

StructuredBuffer<uint> ColorBuffer : register(t1); // assumed to be 32bpp
RWStructuredBuffer<uint> Brightened : register(u2);

[numthreads(32, 32, 1)]
void CS(uint3 DTid : SV_DispatchThreadID )
{
    uint addr = DTid.x + DTid.y * uint(imageWidth);
    uint clr = ColorBuffer[addr];
    uint b = clr & 0xff;
    uint g = (clr >> 8) & 0xff;
    uint r = (clr >> 16) & 0xff;
    r = (r + brightness) & 0xff;
    g = (g + brightness) & 0xff;
    b = (b + brightness) & 0xff;
    clr = (0xff << 24) | (r << 16) | (g << 8) | b;
    Brightened[addr] = clr;
}
