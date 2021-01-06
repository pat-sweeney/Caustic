//**********************************************************************
// Copyright Patrick Sweeney 2020
// All Rights Reserved
// File: AzureLine.vs
//**********************************************************************
struct VSInput
{
    float3 pos : POSITION;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    matrix endpoints;
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
    float maxDepth;

    float minDepth;
    float dummy1;
    float dummy2;
    float dummy3;
    
    int type;
    int colorWidth;
    int colorHeight;
};

float2 Project3D(float3 p)
{
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
    float4 wpos = mul(float4(p.pos, 1.0f), endpoints);
    float3 posDS = mul(float4(wpos.xyz, 1.0f), colorExt).xyz;
    v.pos.xy = Project3D(posDS.xyz);
    v.pos.z = 0.01f;//(depth - minDepth) / (maxDepth - minDepth);
    v.pos.w = 1.0f;
    return v;
}
