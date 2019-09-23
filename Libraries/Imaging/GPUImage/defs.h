//**********************************************************************
// Copyright Patrick Sweeney 2015-2017
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#define MAX_LIGHTS 4

struct VSInput
{
	float3 posOS : POSITION; // Vertex position in object coordinates
	float2 uvs : TEXCOORD0; // Texture coordinates
};

struct VSOutput
{
	float4 posPS : SV_POSITION; // Position of vertex in projected coordinates
	float3 posWS : TEXCOORD0;
	float2 uvs : TEXCOORD2; // UV coordinates
};
