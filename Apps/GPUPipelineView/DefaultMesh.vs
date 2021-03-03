//**********************************************************************
// File: DefaultMesh.vs
//**********************************************************************
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
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 world; // Model => World
    float4x4 worldInvTranspose; // Inverse transpose of world matrix
    float4x4 worldViewProj; // Model => Projected
    float4x4 viewInv; // View => World
    float4 lightPosWS;
    float4x4 colorExt;
    float cx;
    float cy;
    float fx;
    float fy;
    int depthWidth;
    int depthHeight;
    int colorWidth;
    int colorHeight;
    float maxDepth;
    float minDepth;
};

float2 Project3D(float3 p)
{
#pragma warning(disable: 4000)
    if (abs(p.z) < 0.001)
        return float2(0.0f, 0.0f);
    p.x /= p.z;
    p.y /= p.z;
    float2 uvs = float2((p.x * fx + cx) / colorWidth, (p.y * fy + cy) / colorHeight);
    uvs.x = 2.0f * uvs.x - 1.0f;
    uvs.y = 2.0f * (1.0f - uvs.y) - 1.0f;
    return uvs;
}

VSOutput VS(VSInput p)
{
   VSOutput v;

    // Transform our vertex normal from object space to world space
    v.normWS = normalize(mul(float4(p.normOS,1.0f), worldInvTranspose).xyz);
    v.posWS = mul(float4(p.posOS, 1.0f), world).xyz;
    float3 posDS = v.posWS.xyz;//mul(float4(v.posWS.xyz, 1.0f),colorExt);
    //float4 posDS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    v.posPS.xy = Project3D(posDS.xyz);
    v.posPS.z = (posDS.z - minDepth) / (maxDepth - minDepth);
    v.posPS.w = 1.0f;
    return v;
}
