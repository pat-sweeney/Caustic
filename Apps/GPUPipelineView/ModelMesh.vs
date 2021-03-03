//**********************************************************************
// File: ModelMesh.vs
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
    float k1;
    float k2;
    float k3;
    float k4;
    float k5;
    float k6;
    float p1;
    float p2;
    float codx;
    float cody;
    float metricRadius;
    float dummy;
    int type;
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
    float xp = p.x - codx;
    float yp = p.y - cody;
    float xp2 = xp * xp;
    float yp2 = yp * yp;
    float xyp = xp * yp;
    float rs = xp2 + yp2;
#pragma warning(disable:4008)
    if (rs > metricRadius * metricRadius)
        return float2(0.0f / 0.0f, 0.0f / 0.0f);
    float rss = rs * rs;
    float rsc = rss * rs;
    float a = 1.f + k1 * rs + k2 * rss + k3 * rsc;
    float b = 1.f + k4 * rs + k5 * rss + k6 * rsc;
    float bi;
    if (b != 0.f)
        bi = 1.0f / b;
    else
        bi = 1.0f;
    float d = a * bi;
    float xp_d = xp * d;
    float yp_d = yp * d;
    float rs_2xp2 = rs + 2.0f * xp2;
    float rs_2yp2 = rs + 2.0f * yp2;
    if (type == 3)
    {
        xp_d += rs_2xp2 * p2 + xyp * p1;
        yp_d += rs_2yp2 * p1 + xyp * p2;
    }
    else
    {
        // the only difference from Rational6ktCameraModel is 2 multiplier for the tangential coefficient term xyp*p1
        // and xyp*p2
        xp_d += rs_2xp2 * p2 + 2.0f * xyp * p1;
        yp_d += rs_2yp2 * p1 + 2.0f * xyp * p2;
    }
    float xp_d_cx = xp_d + codx;
    float yp_d_cy = yp_d + cody;
    float2 uvs = float2((xp_d_cx * fx + cx) / colorWidth, (yp_d_cy * fy + cy) / colorHeight);
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
    float4 posDS = mul(float4(v.posWS.xyz, 1.0f),colorExt);
    //float4 posDS = mul(float4(p.posOS, 1.0f), worldViewProj);
    v.uvs = p.uvs;
    v.posPS.xy = Project3D(posDS.xyz);
    v.posPS.z = (v.posWS.z - minDepth) / (maxDepth - minDepth);
    v.posPS.w = 1.0f;
    return v;
}
